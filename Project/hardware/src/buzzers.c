#include "buzzers.h"
#include <zephyr/kernel.h>
#include <stdio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>

static struct pwm_dt_spec buzzer1 = PWM_DT_SPEC_GET(DT_ALIAS(buzzer1));
static struct pwm_dt_spec buzzer2 = PWM_DT_SPEC_GET(DT_ALIAS(buzzer2));
static struct pwm_dt_spec buzzer3 = PWM_DT_SPEC_GET(DT_ALIAS(buzzer3));

static bool isInit = false;

/** Specify PWM period in seconds */
#define PWM_SEC(x) (PWM_MSEC(x) * 1000UL)
/** Specify PWM frequency in hertz */
#define PWM_HZ(x) (PWM_SEC(1UL) / (x))

#define period1 PWM_HZ(100)
#define pulse1 period1 * 0.2

#define period2 PWM_HZ(200)
#define pulse2 period2 * 0.2

uint8_t buzzersInit()
{
	if (!pwm_is_ready_dt(&buzzer1))
	{
		printk("Error: PWM device %s is not ready\n",
			   buzzer1.dev->name);
		return 1;
	}
	if (!pwm_is_ready_dt(&buzzer2))
	{
		printk("Error: PWM device %s is not ready\n",
			   buzzer2.dev->name);
		return 1;
	}
	if (!pwm_is_ready_dt(&buzzer3))
	{
		printk("Error: PWM device %s is not ready\n",
			   buzzer3.dev->name);
		return 1;
	}

	return 0;
}

uint8_t buzzerSetPwm(int aBuzzerNum, int aFreq){
	if(aBuzzerNum < 1 || aBuzzerNum > 3){
		printf("Incorrect buzzer number\n");
		return 1;
	}
}

void tbuzzers(void) // Buzzers thread
{
	if (!pwm_is_ready_dt(&buzzer))
	{
		printk("Error: PWM device %s is not ready\n",
			   buzzer.dev->name);
	}
	else
	{
		while (1)
		{
			// buzzer.channel = 1;
			// printk("channel = %i\n", buzzer.channel);
			// pwm_set_dt(&buzzer, period, pulse);
			// k_sleep(K_SECONDS(4));
			// pwm_set_dt(&buzzer, 0, 0);

			// buzzer.channel = 2;
			// printk("channel = %i\n", buzzer.channel);
			// pwm_set_dt(&buzzer, period, pulse);
			// k_sleep(K_SECONDS(4));
			// pwm_set_dt(&buzzer, 0, 0);

			buzzer.channel = 3;
			printk("channel = %i\n", buzzer.channel);
			printk("error while seting is %i\n", pwm_set_dt(&buzzer, period1, pulse1));
			k_sleep(K_SECONDS(4));
			// printk("error while disabling is %i\n", pwm_set_dt(&buzzer, 0, 0));

			// k_sleep(K_SECONDS(1));

			buzzer.channel = 4;
			printk("channel = %i\n", buzzer.channel);
			printk("error while setting is %i\n", pwm_set_dt(&buzzer, period2, pulse2));
			k_sleep(K_SECONDS(4));
			// printk("error while disableing is %i\n", pwm_set_dt(&buzzer, 0, 0));

			// k_sleep(K_SECONDS(1));
		}
	}
}