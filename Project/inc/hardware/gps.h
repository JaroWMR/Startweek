#include <stdint.h>
#include <zephyr/drivers/gnss.h>

void tgps(void);

struct gnss_data getGnssData();
int64_t getLatitude();
int64_t getLongitude();