/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>


void main(void)
{
	const struct device *dev;
	int ret;
	dev = device_get_binding(DT_LABEL(DT_NODELABEL(gpio3)));
	//dev = device_get_binding(DT_GPIO_LABEL(GPIO_3,gpios));
	if(dev == NULL) {
		return;
	}
		

	ret = gpio_pin_configure(dev, 3, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return;
	}

	while (1) {
		gpio_pin_set(dev, 3, 1);
		k_msleep(1000);
		gpio_pin_set(dev, 3, 0);
		k_msleep(1000);
	}
}
