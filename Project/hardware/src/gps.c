#include "gps.h"
#include <zephyr/device.h>
#include <zephyr/drivers/gnss.h>
#include <math.h>
#include <stdio.h>

# define M_PI		3.14159265358979323846	/* pi */

struct gnss_data gpsData;

static void gnss_data_cb(const struct device *dev, const struct gnss_data *data)
{
	if (data->info.fix_status != GNSS_FIX_STATUS_NO_FIX) {
		//printf("%s has fix!\r\n", dev->name);
		int64_t latMilliDeg = data->nav_data.latitude / 1000000;
		int64_t lonMilliDeg = data->nav_data.longitude / 1000000;
		//printf("Lat: %lld millideg\n", latMilliDeg);
		//printf("Lon: %lld millideg\n", lonMilliDeg);
		//printf("Alt: %d mm\n", data->nav_data.altitude);

		gpsData = *data;
	}
}

struct gnss_data getGnssData() {
	return gpsData;
}

int64_t getLatitude() {
	return gpsData.nav_data.latitude;
}

int64_t getLongitude() {
	return gpsData.nav_data.longitude;
}

GNSS_DATA_CALLBACK_DEFINE(DEVICE_DT_GET(DT_ALIAS(gnss)), gnss_data_cb);

#if CONFIG_GNSS_SATELLITES
static void gnss_satellites_cb(const struct device *dev, const struct gnss_satellite *satellites,
			       uint16_t size)
{
	//printf("%s reported %u satellites!\r\n", dev->name, size);
}
#endif

GNSS_SATELLITES_CALLBACK_DEFINE(DEVICE_DT_GET(DT_ALIAS(gnss)), gnss_satellites_cb);

// Utility function for 
// converting degrees to radians
long double toRadians(const long double degree)
{
    // cmath library in C++ 
    // defines the constant
    // M_PI as the value of
    // pi accurate to 1e-30
    long double one_deg = (M_PI) / 180;
    return (one_deg * degree);
}
 
long double getDistance(long double lat1, long double long1, 
                     long double lat2, long double long2)
{
    // Convert the latitudes 
    // and longitudes
    // from degree to radians.
    lat1 = toRadians(lat1);
    long1 = toRadians(long1);
    lat2 = toRadians(lat2);
    long2 = toRadians(long2);
     
    // Haversine Formula
    long double dlong = long2 - long1;
    long double dlat = lat2 - lat1;
 
    long double ans = pow(sin(dlat / 2), 2) + 
                          cos(lat1) * cos(lat2) * 
                          pow(sin(dlong / 2), 2);
 
    ans = 2 * asin(sqrt(ans));
 
    // Radius of Earth in 
    // Kilometers, R = 6371
    // Use R = 3956 for miles
    long double R = 6371;
     
    // Calculate the result
    ans = ans * R;
 
    return ans;
}