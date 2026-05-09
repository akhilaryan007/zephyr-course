// #include <zephyr/drivers/gpio.h>
// #include <zephyr/kernel.h>
// #include <zephyr/logging/log.h>
// #include <zephyr/drivers/sensor.h>

// #define MY_DEV DT_NODELABEL(our_driver)

// #define SLEEP_TIME_MS 1000

// /* The devicetree node identifier for the "led0" alias. */
// #define LED_NODE DT_ALIAS(app_led)

// static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

// LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

// // namespace {
// //     void test() {
// //         const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(our_driver));
// //         struct sensor_value val;
// //         auto ret = sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &val);
// //         LOG_INF("channel ret %d", ret);
// //     }   // You can add any additional functions or variables here if needed
// // }

// void test_sensor(void)
// {
//     const struct device *dev = DEVICE_DT_GET(MY_DEV);

//     struct sensor_value val;

//     sensor_sample_fetch(dev);
//     sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &val);

// }

// extern "C" void testable_sensor(void);

// int main(void)
// {

//     bool led_state = true;

//     if (!gpio_is_ready_dt(&led)) return 0;

//     if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;

//     while (1) {
//         if (gpio_pin_toggle_dt(&led) < 0) return 0;

//         led_state = !led_state;
//         LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
//         LOG_INF("Iomico course setup complete!");
//         //k_msleep(CONFIG_BLINK_SLEEP_TIME_MS); 
//         k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);  
//     }
//     testable_sensor();
//     return 0;
// }

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/sys/printk.h>

#include "our_driver.h"

#define MY_DEV DT_NODELABEL(our_driver0)

int main(void)
{
    const struct device *dev = DEVICE_DT_GET(MY_DEV);

    if (!device_is_ready(dev)) {
        printk("Device not ready\n");
        return 0;
    }

    printk("Device ready\n");

    struct sensor_value val;

    /* ---------------------------------- */
    /* SENSOR API */
    /* ---------------------------------- */

    sensor_sample_fetch(dev);

    sensor_channel_get(dev,
                       SENSOR_CHAN_ALL,
                       &val);

    printk("Sensor Value = %d\n", val.val1);

    /* ---------------------------------- */
    /* CUSTOM EXTENSION API */
    /* ---------------------------------- */

    const struct my_sensor_api *api =
        (const struct my_sensor_api *)dev->api;

    api->set_led_state(dev, 1);

    sensor_channel_get(dev,
                       SENSOR_CHAN_ALL,
                       &val);

    printk("Updated Sensor Value = %d\n",
           val.val1);

    return 0;
}
