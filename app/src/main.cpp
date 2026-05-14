#include <stdio.h>

#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zephyr/drivers/sensor.h>

/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(app_led)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

namespace {
    void test() {
        const struct device* dev = DEVICE_DT_GET(DT_NODELABEL(my_driver0));

        if (!device_is_ready(dev)) {
            return;
        }

        struct sensor_value val;
        int ret = sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &val);
        LOG_INF("Channel ret: %d", ret);

        if (ret == 0) {
            // Use sensor_value_to_double(&val) or similar to read the data
        }


    }


}


int main(void)
{
    test();
    bool led_state = true;

    if (!gpio_is_ready_dt(&led)) return 0;

    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;

    while (1) {
        if (gpio_pin_toggle_dt(&led) < 0) return 0;

        led_state = !led_state;
        LOG_INF("LED state: %s", led_state ? "ON" : "OFF");

        printf("Hello World! %s\n", CONFIG_BOARD_TARGET);

        k_msleep(CONFIG_APP_HEART_BEAT_PERIOD_MS);
    }
    return 0;
}
