
#include <zephyr/drivers/sensor.h>

void testable_sensor(void)
{
    const struct device *dev =
        DEVICE_DT_GET(DT_NODELABEL(our_driver0));

    if (!device_is_ready(dev)) {
        printk("Device not ready\n");
        return;
    }

    printk("Sensor ready\n");
}