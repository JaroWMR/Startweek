#include "headingCalculator.h"
#include <stdio.h>
#include <stdint.h>
#include <math.h>

/* roll pitch and yaw angles computed by iecompass */
static int16_t iPhi, iThe, iPsi;
/* magnetic field readings corrected for hard iron effects and PCB orientation */
static int16_t iBfx, iBfy, iBfz;
/* hard iron estimate */
static int16_t iVx, iVy, iVz;

int32_t tmpAngle;		   /* temporary angle*100 deg: range -36000 to 36000 */
static int16_t iLPPsi;	   /* low pass filtered angle*100 deg: range -18000 to 18000 */
static uint16_t ANGLE_LPF; /* low pass filter: set to 32768 / N for N samples averaging */

#define MINDELTATRIG 1

/* Fifth order polynomial approximation giving 0.05 deg max error */
const int16_t K1 = 5701;
const int16_t K2 = -1645;
const int16_t K3 = 446;

/* Final step size for iDivide */
const uint16_t MINDELTADIV = 1;

/* Function to calculate ir = iy / ix with iy <= ix, and ix, iy both > 0 */
static int16_t iDivide(int16_t iy, int16_t ix)
{
	int16_t itmp;	/* Scratch */
	int16_t ir;		/* Result = iy / ix, range 0. to 1., returned in range 0 to 32767 */
	int16_t idelta; /* Delta on candidate result, dividing each stage by factor of 2 */

	/* Set result r to zero and binary search step to 16384 = 0.5 */
	ir = 0;
	idelta = 16384; /* Set as 2^14 = 0.5 */

	/* To reduce quantization effects, boost ix and iy to the maximum signed 16 bit value */
	while ((ix < 16384) && (iy < 16384))
	{
		ix += ix;
		iy += iy;
	}

	/* Loop over binary sub-division algorithm solving for ir*ix = iy */
	do
	{
		/* Generate new candidate solution for ir and test if we are too high or too low */
		itmp = ir + idelta; /* itmp=ir+delta, the candidate solution */
		itmp = (int16_t)((itmp * ix) >> 15);
		if (itmp <= iy)
			ir += idelta;
		idelta >>= 1; /* Divide by 2 using right shift one bit */
	} while (idelta >= MINDELTADIV); /* Last loop is performed for idelta=MINDELTADIV */

	return ir;
}

/* Calculates 100*atan(iy/ix) range 0 to 9000 for all ix, iy positive in range 0 to 32767 */
static int16_t iHundredAtanDeg(int16_t iy, int16_t ix)
{
	int32_t iAngle; /* Angle in degrees times 100 */
	int16_t iRatio; /* Ratio of iy / ix or vice versa */
	int32_t iTmp;	/* Temporary variable */

	/* Check for pathological cases */
	if ((ix == 0) && (iy == 0))
		return 0;
	if ((ix == 0) && (iy != 0))
		return 9000;

	/* Check for non-pathological cases */
	if (iy <= ix)
		iRatio = iDivide(iy, ix); /* return a fraction in range 0. to 32767 = 0. to 1. */
	else
		iRatio = iDivide(ix, iy); /* return a fraction in range 0. to 32767 = 0. to 1. */

	/* First, third, and fifth order polynomial approximation */
	iAngle = (int32_t)K1 * (int32_t)iRatio;
	iTmp = ((int32_t)iRatio >> 5) * ((int32_t)iRatio >> 5) * ((int32_t)iRatio >> 5);
	iAngle += (iTmp >> 15) * (int32_t)K2;
	iTmp = (iTmp >> 20) * ((int32_t)iRatio >> 5) * ((int32_t)iRatio >> 5);
	iAngle += (iTmp >> 15) * (int32_t)K3;
	iAngle = iAngle >> 15;

	/* Check if above 45 degrees */
	if (iy > ix)
		iAngle = 9000 - iAngle;

	/* For tidiness, limit result to range 0 to 9000 equals 0.0 to 90.0 degrees */
	if (iAngle < 0)
		iAngle = 0;
	if (iAngle > 9000)
		iAngle = 9000;

	return (int16_t)iAngle;
}

/* Calculates 100*atan2(iy/ix)=100*atan2(iy,ix) in degrees for ix, iy in range -32768 to 32767 */
static int16_t iHundredAtan2Deg(int16_t iy, int16_t ix)
{
	int16_t iResult; /* angle in degrees times 100 */

	/* Check for -32768 which is not handled correctly */
	if (ix == -32768)
		ix = -32767;
	if (iy == -32768)
		iy = -32767;

	/* Check for quadrants */
	if ((ix >= 0) && (iy >= 0)) /* Range 0 to 90 degrees */
		iResult = iHundredAtanDeg(iy, ix);
	else if ((ix <= 0) && (iy >= 0)) /* Range 90 to 180 degrees */
		iResult = (int16_t)(18000 - (int16_t)iHundredAtanDeg(iy, (int16_t)-ix));
	else if ((ix <= 0) && (iy <= 0)) /* Range -180 to -90 degrees */
		iResult = (int16_t)((int16_t)-18000 + iHundredAtanDeg((int16_t)-iy, (int16_t)-ix));
	else /* ix >=0 and iy <= 0 giving range -90 to 0 degrees */
		iResult = (int16_t)(-iHundredAtanDeg((int16_t)-iy, ix));

	return iResult;
}

/* function to calculate ir = ix / sqrt(ix*ix+iy*iy) using binary division */
static int16_t iTrig(int16_t ix, int16_t iy)
{
	uint32_t itmp;	 /* scratch */
	uint32_t ixsq;	 /* ix * ix */
	int16_t isignx;	 /* storage for sign of x. algorithm assumes x >= 0 then corrects later */
	uint32_t ihypsq; /* (ix * ix) + (iy * iy) */
	int16_t ir;		 /* result = ix / sqrt(ix*ix+iy*iy) range -1, 1 returned as signed int16 */
	int16_t idelta;	 /* delta on candidate result dividing each stage by factor of 2 */

	/* correct for pathological case: ix==iy==0 */
	if ((ix == 0) && (iy == 0))
		ix = iy = 1;

	/* check for -32768 which is not handled correctly */
	if (ix == -32768)
		ix = -32767;
	if (iy == -32768)
		iy = -32767;

	/* store the sign for later use. algorithm assumes x is positive for convenience */
	isignx = 1;
	if (ix < 0)
	{
		ix = (int16_t)-ix;
		isignx = -1;
	}

	/* for convenience in the boosting set iy to be positive as well as ix */
	iy = (int16_t)abs(iy);

	/* to reduce quantization effects, boost ix and iy but keep below maximum signed 16 bit */
	while ((ix < 16384) && (iy < 16384))
	{
		ix += ix;
		iy += iy;
	}

	/* calculate ix*ix and the hypotenuse squared */
	ixsq = (uint32_t)(ix * ix);			 /* ixsq=ix*ix: 0 to 32767^2 = 1073676289 */
	ihypsq = (uint32_t)(ixsq + iy * iy); /* ihypsq=(ix*ix+iy*iy) 0 to 2*32767*32767=2147352578 */

	/* set result r to zero and binary search step to 16384 = 0.5 */
	ir = 0;
	idelta = 16384; /* set as 2^14 = 0.5 */

	/* loop over binary sub-division algorithm */
	do
	{
		/* generate new candidate solution for ir and test if we are too high or too low */
		/* itmp=(ir+delta)^2, range 0 to 32767*32767 = 2^30 = 1073676289 */
		itmp = ((uint32_t)(ir + idelta) * (ir + idelta));
		/* itmp=(ir+delta)^2*(ix*ix+iy*iy), range 0 to 2^31 = 2147221516 */
		itmp = (itmp >> 15) * (ihypsq >> 15);
		if (itmp <= ixsq)
			ir += idelta;
		idelta >>= 1; /* divide by 2 using right shift one bit */
	} while (idelta >= MINDELTATRIG); /* last loop is performed for idelta=MINDELTATRIG */

	/* correct the sign before returning */
	return (int16_t)(ir * isignx);
}

/* Implement a modulo arithmetic exponential low pass filter on the yaw angle */
void implementLPF()
{
	/* Compute the change in angle modulo 360 degrees */
	tmpAngle = (int32_t)iPsi - (int32_t)iLPPsi;
	if (tmpAngle > 18000)
		tmpAngle -= 36000;
	if (tmpAngle < -18000)
		tmpAngle += 36000;
	/* Calculate the new low pass filtered angle */
	tmpAngle = (int32_t)iLPPsi + ((ANGLE_LPF * tmpAngle) >> 15);
	/* Check that the angle remains in -180 to 180 deg bounds */
	if (tmpAngle > 18000)
		tmpAngle -= 36000;
	if (tmpAngle < -18000)
		tmpAngle += 36000;

	// if (tmpAngle > 9000)
	// 	tmpAngle = (Int16)(18000 - tmpAngle);
	// if (tmpAngle < -9000)
	// 	tmpAngle = (Int16)(-18000 - tmpAngle);

	/* Store the correctly bounded low pass filtered angle */
	iLPPsi = (int16_t)tmpAngle;
}

void iecompass(int16_t iBpx, int16_t iBpy, int16_t iBpz,
			   int16_t iGpx, int16_t iGpy, int16_t iGpz)
{
	/* stack variables */
	/* iBpx, iBpy, iBpz: the three components of the magnetometer sensor */
	/* iGpx, iGpy, iGpz: the three components of the accelerometer sensor */
	/* local variables */
	int16_t iSin, iCos; /* sine and cosine */
	/* subtract the hard iron offset */
	iBpx -= iVx; /* see Eq 16 */
	iBpy -= iVy; /* see Eq 16 */
	iBpz -= iVz; /* see Eq 16 */

	/* calculate current roll angle Phi */
	iPhi = iHundredAtan2Deg(iGpy, iGpz); /* Eq 13 */
	/* calculate sin and cosine of roll angle Phi */
	iSin = iTrig(iGpy, iGpz); /* Eq 13: sin = opposite / hypotenuse */
	iCos = iTrig(iGpz, iGpy); /* Eq 13: cos = adjacent / hypotenuse */
	/* de-rotate by roll angle Phi */
	iBfy = (int16_t)((iBpy * iCos - iBpz * iSin) >> 15); /* Eq 19 y component */
	iBpz = (int16_t)((iBpy * iSin + iBpz * iCos) >> 15); /* Bpy*sin(Phi)+Bpz*cos(Phi)*/
	iGpz = (int16_t)((iGpy * iSin + iGpz * iCos) >> 15); /* Eq 15 denominator */

	/* calculate current pitch angle Theta */
	iThe = iHundredAtan2Deg((int16_t)-iGpx, iGpz); /* Eq 15 */
	/* restrict pitch angle to range -90 to 90 degrees */
	if (iThe > 9000)
		iThe = (int16_t)(18000 - iThe);
	if (iThe < -9000)
		iThe = (int16_t)(-18000 - iThe);
	/* calculate sin and cosine of pitch angle Theta */
	iSin = (int16_t)-iTrig(iGpx, iGpz); /* Eq 15: sin = opposite / hypotenuse */
	iCos = iTrig(iGpz, iGpx);			/* Eq 15: cos = adjacent / hypotenuse */
	/* correct cosine if pitch not in range -90 to 90 degrees */
	if (iCos < 0)
		iCos = (int16_t)-iCos;
	/* de-rotate by pitch angle Theta */
	iBfx = (int16_t)((iBpx * iCos + iBpz * iSin) >> 15);  /* Eq 19: x component */
	iBfz = (int16_t)((-iBpx * iSin + iBpz * iCos) >> 15); /* Eq 19: z component */

	/* calculate current yaw = e-compass angle Psi */
	iPsi = iHundredAtan2Deg((int16_t)-iBfy, iBfx); /* Eq 22 */
}
