#ifndef OUR_DRIVER_H
#define OUR_DRIVER_H

#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

typedef int (*set_led_state_t)(const struct device *dev,
                               int state);

struct my_sensor_api {
    struct sensor_driver_api sensor_api;

    set_led_state_t set_led_state;
};

#endif