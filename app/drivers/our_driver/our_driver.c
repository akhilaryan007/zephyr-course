// // #define DT_DRV_COMPAT iomico_my_sensor

// // #include <zephyr/drivers/sensor.h>
// // //#include<zephyr/drivers/sensor.h>
// // #include<zephyr/logging/log.h>


// // LOG_MODULE_DECLARE(our_driver, LOG_LEVEL_INF);

// // static int channel_get_my_impl(const struct device *dev,
// //                               enum sensor_channel chan,
// //                               struct sensor_value *val)
// // {
// //     LOG_INF("channel %d", chan);
// //     return 0;
// // }


// // static DEVICE_API(sensor, api_iomico)= {
// //     .channel_get = channel_get_my_impl,

// // };

// // static int init(const struct device *dev)
// // {
// //     LOG_INF("Device initialized");
// //     return 0;
// // }

// // DEVICE_DT_INST_DEFINE(0, init, NULL, NULL, NULL, POST_KERNEL, 80, &api_iomico);


// #define DT_DRV_COMPAT iomico_my_sensor

// #include <zephyr/drivers/sensor.h>
// #include <zephyr/logging/log.h>

// LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

// static int channel_get_my_impl(const struct device *dev,
//                               enum sensor_channel chan,
//                               struct sensor_value *val)
// {
//     LOG_INF("channel %d", chan);
//     return 0;
// }

// static DEVICE_API(sensor, api_iomico) = {
//     .channel_get = channel_get_my_impl,
// };

// static int init(const struct device *dev)
// {
//     LOG_INF("Device initialized");
//     return 0;
// }

// #define DEV_INST(inst) 
// DEVICE_DT_INST_DEFINE(0,
//                       inst,
//                       NULL,
//                       NULL,
//                       NULL,
//                       POST_KERNEL,
//                       80,
//                       &api_iomico);

// DT_INST_FOREACH_STATUS_OKAY(DEV_INST);


#define DT_DRV_COMPAT iomico_sensor

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#include "our_driver.h"

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

struct my_sensor_data {
    int led_state;
};

/* ------------------------------------------------- */
/* SENSOR API IMPLEMENTATION */
/* ------------------------------------------------- */

static int sample_fetch_my_impl(const struct device *dev,
                                enum sensor_channel chan)
{
    struct my_sensor_data *data = dev->data;

    data->led_state = !data->led_state;

    LOG_INF("LED toggled to %d", data->led_state);

    return 0;
}

static int channel_get_my_impl(const struct device *dev,
                               enum sensor_channel chan,
                               struct sensor_value *val)
{
    struct my_sensor_data *data = dev->data;

    val->val1 = data->led_state;
    val->val2 = 0;

    LOG_INF("Channel Get -> LED State: %d",
            data->led_state);

    return 0;
}

/* ------------------------------------------------- */
/* CUSTOM EXTENSION API */
/* ------------------------------------------------- */

static int my_set_led_state(const struct device *dev,
                            int state)
{
    struct my_sensor_data *data = dev->data;

    data->led_state = state;

    LOG_INF("Custom API changed LED state to %d",
            state);

    return 0;
}

/* ------------------------------------------------- */
/* DRIVER API */
/* ------------------------------------------------- */

static struct my_sensor_api api_iomico = {
    .sensor_api = {
        .sample_fetch = sample_fetch_my_impl,
        .channel_get = channel_get_my_impl,
    },

    .set_led_state = my_set_led_state,
};

/* ------------------------------------------------- */
/* INIT */
/* ------------------------------------------------- */

static int init(const struct device *dev)
{
    LOG_INF("Device initialized");

    return 0;
}

/* ------------------------------------------------- */
/* DEVICE INSTANTIATION */
/* ------------------------------------------------- */

#define DEV_INST(inst)                                  \
    static struct my_sensor_data my_data_##inst;        \
                                                        \
    DEVICE_DT_INST_DEFINE(inst,                         \
                          init,                         \
                          NULL,                         \
                          &my_data_##inst,              \
                          NULL,                         \
                          POST_KERNEL,                  \
                          80,                           \
                          (const struct sensor_driver_api *)&api_iomico);

DT_INST_FOREACH_STATUS_OKAY(DEV_INST)