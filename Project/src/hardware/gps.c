#include "gps.h"
#include <stdio.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gnss.h>

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

void tgps(void) // gps thread
{
	// This thread is actually unnecessary because gps uses interrupts
	//while (1) printf("Reading GPS\n");
}

double getDistToLoc(struct gnss_data coord) {
	return 0;
}