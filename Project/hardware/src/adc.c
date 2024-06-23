// #include "adc.h"
// #include <inttypes.h>
// #include <stddef.h>
// #include <stdint.h>

// #include <zephyr/device.h>
// #include <zephyr/devicetree.h>
// #include <zephyr/drivers/adc.h>
// #include <zephyr/kernel.h>
// #include <zephyr/sys/printk.h>
// #include <zephyr/sys/util.h>


// bool adcConfig()
// {
// 	int err;
// 	/* Configure channels individually prior to sampling. */
// 	for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
// 		if (!adc_is_ready_dt(&adc_channels[i])) {
// 			printk("ADC controller device %s not ready\n", adc_channels[i].dev->name);
// 			return 0;
// 		}

// 		err = adc_channel_setup_dt(&adc_channels[i]);
// 		if (err < 0) {
// 			printk("8Could not setup channel #%d (%d)\n", i, err);
// 			return 0;
// 		}
// 	}
// }
// int32_t adcGetPotmeter()
// {
// 	int channelSelect = 0;
// 	int err = 0;
// 	uint16_t buf;
// 	int32_t val_mv;
// 	struct adc_sequence sequence = {
// 		.buffer = &buf,
// 		/* buffer size in bytes, not number of samples */
// 		.buffer_size = sizeof(buf),
// 	};
// 	(void)adc_sequence_init_dt(&adc_channels[channelSelect], &sequence);

// 	err = adc_read_dt(&adc_channels[channelSelect], &sequence);
// 	if (err < 0) {
// 		printk("Could not read (%d)\n", err);
// 	}
// 	if (adc_channels[channelSelect].channel_cfg.differential) 
// 	{
// 		val_mv = (int32_t)((int16_t)buf);
// 	} else {
// 		val_mv = (int32_t)buf;
// 	}
// 	adc_raw_to_millivolts_dt(&adc_channels[channelSelect],&val_mv);
// 	return val_mv;
// }
// int32_t adcGetPower()
// {
// 	int channelSelect = 0;
// 	int err = 0;
// 	uint16_t buf;
// 	int32_t val_mv;
// 	struct adc_sequence sequence = {
// 		.buffer = &buf,
// 		/* buffer size in bytes, not number of samples */
// 		.buffer_size = sizeof(buf),
// 	};
// 	(void)adc_sequence_init_dt(&adc_channels[channelSelect], &sequence);

// 	err = adc_read_dt(&adc_channels[channelSelect], &sequence);
// 	if (err < 0) {
// 		printk("Could not read (%d)\n", err);
// 	}
// 	if (adc_channels[channelSelect].channel_cfg.differential) 
// 	{
// 		val_mv = (int32_t)((int16_t)buf);
// 	} else {
// 		val_mv = (int32_t)buf;
// 	}
// 	adc_raw_to_millivolts_dt(&adc_channels[channelSelect],&val_mv);
// 	return val_mv;
// }