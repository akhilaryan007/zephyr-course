// #define DT_DRV_COMPAT iomico_my_sensor

// #include <zephyr/drivers/sensor.h>
// //#include<zephyr/drivers/sensor.h>
// #include<zephyr/logging/log.h>


// LOG_MODULE_DECLARE(our_driver, LOG_LEVEL_INF);

// static int channel_get_my_impl(const struct device *dev,
//                               enum sensor_channel chan,
//                               struct sensor_value *val)
// {
//     LOG_INF("channel %d", chan);
//     return 0;
// }


// static DEVICE_API(sensor, api_iomico)= {
//     .channel_get = channel_get_my_impl,

// };

// static int init(const struct device *dev)
// {
//     LOG_INF("Device initialized");
//     return 0;
// }

// DEVICE_DT_INST_DEFINE(0, init, NULL, NULL, NULL, POST_KERNEL, 80, &api_iomico);


#define DT_DRV_COMPAT iomico_sensor

#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

static int channel_get_my_impl(const struct device *dev,
                              enum sensor_channel chan,
                              struct sensor_value *val)
{
    LOG_INF("channel %d", chan);
    return 0;
}

static DEVICE_API(sensor, api_iomico) = {
    .channel_get = channel_get_my_impl,
};

static int init(const struct device *dev)
{
    LOG_INF("Device initialized");
    return 0;
}

DEVICE_DT_INST_DEFINE(0,
                      init,
                      NULL,
                      NULL,
                      NULL,
                      POST_KERNEL,
                      80,
                      &api_iomico);