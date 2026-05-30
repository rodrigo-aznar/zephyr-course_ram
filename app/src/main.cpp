#include <stdio.h>

#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zephyr/drivers/sensor.h>

#include "my_driver.h"

/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(app_led)

/* Lecture 7 - Home task 1 - no circular LED blinking - 
   LEDs are controlled only by shell subcmds:

    uart:~$ sensor read my_driver
    SUB CMD read / channel_get (Turn LED OFF)

    uart:~$ sensor fetch my_driver
    SUB CMD fetch / sample_fetch (Turn LED ON)

    uart:~$ sensor info my_driver
    SUB CMD info (prints device name directly from hardware device tree)
    Successfully found device: my_driver

*/
#define LECTURE_NR 71 /* Lecture 7 - Home task 1 */

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    #if (LECTURE_NR >= 6)
    struct sensor_value value;
    const struct device *mydev = DEVICE_DT_GET(DT_NODELABEL(my_driver0));

    if (!device_is_ready(mydev)) {
        LOG_ERR("Error: My driver device is not ready!\n");
        return -ENODEV; 
    }
    #endif

    uint32_t default_sleep_ms = my_get_sleep_ms(mydev);

    bool led_state = true;
    #if (LECTURE_NR == 6)
    uint32_t sequential_max = 5; // max number of changes we allow sleep to change (always by double)
    uint32_t sleep_iteration_change = 0;
    #endif
    uint32_t current_sleep_ms = 0;

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

        #if (LECTURE_NR == 6)

        if (led_state)
            (void)sensor_sample_fetch(mydev);
        else
            (void)sensor_channel_get(mydev, SENSOR_CHAN_AMBIENT_TEMP, &value);

        #endif

        #if (LECTURE_NR <= 6)
        LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
        #endif

        current_sleep_ms = my_get_sleep_ms(mydev);
        k_msleep(current_sleep_ms);

        #if (LECTURE_NR == 6)
        // double the sleep_ms each time until sequential_max is reached
        if (++sleep_iteration_change <= sequential_max)
        {
            my_set_sleep_ms(mydev, 2*current_sleep_ms);
            LOG_INF("_iteration: %d  sleep_ms_after doubled: %d ", sleep_iteration_change, my_get_sleep_ms(mydev));
        }
        else
        {   // reset to initial condition to restart the cycle
            sleep_iteration_change = 0;
            my_set_sleep_ms(mydev, default_sleep_ms);
            LOG_INF("  Reset to default sleep value: ", my_get_sleep_ms(mydev));
        }
        #endif

    }
    return 0;
}
