#include <stdint.h>
#include <zephyr/drivers/gnss.h>

void tgps(void);
int64_t getLatitude();
int64_t getLongitude();

double getDistToLoc(struct gnss_data coord);