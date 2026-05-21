#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/shell/shell.h>
#include "our_driver.h"
#include <stdlib.h>

static const struct device *sensor_dev =
    DEVICE_DT_GET(DT_NODELABEL(our_driver0));
    
static int cmd_sensor_info(const struct shell *sh,
                           size_t argc,
                           char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    shell_print(sh, "Device: %s", sensor_dev->name);
    shell_print(sh, "Ready: %s",
                device_is_ready(sensor_dev) ? "YES" : "NO");

    return 0;
}
static int cmd_sensor_fetch(const struct shell *sh,
                            size_t argc,
                            char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    int ret = sensor_sample_fetch(sensor_dev);

    if (ret < 0) {
        shell_error(sh, "Fetch failed: %d", ret);
        return ret;
    }

    shell_print(sh, "Sample fetched");

    return 0;
}


static int cmd_sensor_read(const struct shell *sh,
                           size_t argc,
                           char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    struct sensor_value val;

    int ret = sensor_channel_get(sensor_dev,
                                 SENSOR_CHAN_LIGHT,
                                 &val);

    if (ret < 0) {
        shell_error(sh, "Read failed: %d", ret);
        return ret;
    }

    shell_print(sh,
                "Value: %d.%06d",
                val.val1,
                val.val2);

    return 0;
}


//TASK-2: Add a new shell command to set the LED state using the custom API

static int cmd_sensor_set(const struct shell *sh,
                          size_t argc,
                          char **argv)
{
    ARG_UNUSED(argc);

    if (!device_is_ready(sensor_dev)) {
        shell_error(sh, "Device not ready");
        return -ENODEV;
    }

    long value = strtol(argv[1], NULL, 10);


    if (value < 0 || value > 1) {
        shell_error(sh,
                    "Invalid value. Use 0 or 1");
        return -EINVAL;
    }

    const struct my_sensor_api *api =
        (const struct my_sensor_api *)sensor_dev->api;

    if (api->set_led_state == NULL) {
        shell_error(sh,
                    "Custom API not implemented");
        return -ENOSYS;
    }

    int ret = api->set_led_state(sensor_dev,
                                 (int)value);

    if (ret < 0) {
        shell_error(sh,
                    "Failed to set LED state");
        return ret;
    }

    shell_print(sh,
                "LED state updated to %ld",
                value);

    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sensor_cmds,
    SHELL_CMD_ARG(fetch, NULL, "Fetch sensor sample", cmd_sensor_fetch, 1, 0),
    SHELL_CMD_ARG(read, NULL, "Read sensor value", cmd_sensor_read, 1, 0),
    SHELL_CMD_ARG(info, NULL, "Sensor info", cmd_sensor_info, 1, 0),
    SHELL_CMD_ARG(set, NULL, "Set sensor state", cmd_sensor_set, 2, 0),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor,
                   &sensor_cmds,
                   "Sensor commands",
                   NULL);

