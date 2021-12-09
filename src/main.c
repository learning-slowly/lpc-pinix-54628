/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <drivers/pwm.h>
#include <sys/printk.h>


#define MIN_PERIOD_USEC	(USEC_PER_SEC / 1024U) //64Hz 
#define MAX_PERIOD_USEC	USEC_PER_SEC  //1Hz //500 msec 

#define PWM_FLAGS	DT_PWMS_FLAGS(DT_NODELABEL(red_pwm_led))

void main(void)
{
	const struct device *dev, *pwm_led;
	int ret;
	int status;
	dev = device_get_binding(DT_LABEL(DT_NODELABEL(gpio3)));
	//dev = device_get_binding(DT_GPIO_LABEL(GPIO_3,gpios));
	if(dev == NULL) {
		return;
	}

	pwm_led = device_get_binding(DT_LABEL(DT_NODELABEL(sc_timer)));
	if (!device_is_ready(pwm_led)) {
		printk("Error: PWM device %s is not ready\n", pwm_led->name);
		return;
	}

	ret = gpio_pin_configure(dev, 3, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return;
	}

	gpio_pin_set(dev, 3, 0);
	status = pwm_pin_set_usec(pwm_led, 6,MAX_PERIOD_USEC, MAX_PERIOD_USEC/2, PWM_FLAGS);
	while (1) {
		printk("hello world!(%d)\n",status);
		k_msleep(20000);

		//status = pwm_pin_set_usec(pwm_led, 6,MIN_PERIOD_USEC, MIN_PERIOD_USEC/2, PWM_FLAGS);
		printk("hello world!(%d)\n",status);
		//gpio_pin_set(dev, 3, 0);
		k_msleep(20000);
	}
}
