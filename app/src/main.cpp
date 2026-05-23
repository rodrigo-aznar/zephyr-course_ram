#include <stdio.h>

#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zephyr/drivers/sensor.h>

/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(app_led)

#define LECTURE_NR 6

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

namespace {
    int test() {
        const struct device* dev = DEVICE_DT_GET(DT_NODELABEL(my_driver0));

        if (!device_is_ready(dev)) {
            return -ENODEV;
        }

        struct sensor_value val;
        int ret = sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &val);
        LOG_INF("Channel ret: %d", ret);

        if (ret == 0) {
            // Use sensor_value_to_double(&val) or similar to read the data
        }
        return ret;
    }
}


int main(void)
{
    #if (LECTURE_NR >= 6)
    struct sensor_value value;
    const struct device *mydev = DEVICE_DT_GET(DT_NODELABEL(my_driver0));

    if (!device_is_ready(mydev)) {
        LOG_ERR("Error: My driver device is not ready!\n");
        return -ENODEV; 
    }

    /* JUST FOR BASIC TEST */ test();
    #endif


    bool led_state = true;


    #if (LECTURE_NR < 6) // lessons previous to custom driver
    if (!gpio_is_ready_dt(&led)) return 0;

    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;
    #endif

    LOG_INF("Hello World! %s\n", CONFIG_BOARD_TARGET);

    while (1) {

        #if (LECTURE_NR < 6) // lessons previous to custom driver
        if (gpio_pin_toggle_dt(&led) < 0) return 0;
        #endif

        led_state = !led_state;

        #if (LECTURE_NR >= 6)

        if (led_state)
            (void)sensor_sample_fetch(mydev);
        else
            (void)sensor_channel_get(mydev, SENSOR_CHAN_AMBIENT_TEMP, &value);

        #endif

        LOG_INF("LED state: %s", led_state ? "ON" : "OFF");

        k_msleep(CONFIG_APP_HEART_BEAT_PERIOD_MS);
    }
    return 0;
}
