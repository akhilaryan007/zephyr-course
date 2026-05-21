#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/shell/shell.h>

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


SHELL_STATIC_SUBCMD_SET_CREATE(sensor_cmds,
    SHELL_CMD(fetch, NULL, "Fetch sensor sample", cmd_sensor_fetch),
    SHELL_CMD(read, NULL, "Read sensor value", cmd_sensor_read),
    SHELL_CMD(info, NULL, "Sensor info", cmd_sensor_info),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor,
                   &sensor_cmds,
                   "Sensor commands",
                   NULL);

