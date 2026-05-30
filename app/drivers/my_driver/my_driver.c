#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>

#define DT_DRV_COMPAT ram_driver

LOG_MODULE_REGISTER(sensor_ram_driver, LOG_LEVEL_INF);

/* static data/config */
struct my_driver_config {
    struct gpio_dt_spec led;
    uint32_t default_sleep_ms;
};

/* dynamic data */
struct my_driver_data {
    uint32_t sleep_ms;
};

// custom function 1
uint32_t my_get_sleep_ms(const struct device *dev) {
    struct my_driver_data *data = dev->data;
    return data->sleep_ms;
}

// custom function 2
void my_set_sleep_ms(const struct device *dev, uint32_t sleep_ms_to_set) {
    struct my_driver_data *data = dev->data;
    data->sleep_ms = sleep_ms_to_set;
    LOG_INF(" sleep_ms updated to %d \n", data->sleep_ms);
}

static int my_sample_fetch(const struct device *dev, enum sensor_channel chan) {
    const struct my_driver_config *cfg = dev->config;
    // LOG_INF(" Turn LED ON *** \n");
    gpio_pin_set_dt(&cfg->led, 1);
    return 0;
}


static int my_get_channel(const struct device *dev, enum sensor_channel chan, struct sensor_value *val) {
    const struct my_driver_config *cfg = dev->config;
    // LOG_INF(" Turn LED OFF --- \n");
    gpio_pin_set_dt(&cfg->led, 0);
    return 0;
}


static DEVICE_API(sensor, api_my_driver) = {
    .channel_get  = my_get_channel,
    .sample_fetch = my_sample_fetch,    
};

static int init(const struct device* dev) {
    const struct my_driver_config *cfg = dev->config;
    struct my_driver_data *data = dev->data;

    data->sleep_ms = cfg->default_sleep_ms;

    gpio_pin_configure_dt(&cfg->led, GPIO_OUTPUT_ACTIVE);    
    
    LOG_INF(" -- Device Initialized -- ");

    return 0;
}


// Automatically instantiate a distinct driver instance for every enabled 
// node matching this driver's compatible string in the devicetree.
#define DEV_INST(inst)                                 \
    static struct my_driver_data my_driver_data_##inst = { \
        .sleep_ms = DT_INST_PROP(inst, sleep_ms),      \
    }; \
    static const struct my_driver_config my_driver_config_##inst = { \
        .default_sleep_ms = DT_INST_PROP(inst, default_sleep_ms),      \
        .led = GPIO_DT_SPEC_INST_GET(inst, gpios),     \
    };                                                 \
    DEVICE_DT_INST_DEFINE(inst, init, NULL,            \
                          &my_driver_data_##inst,      \
                          &my_driver_config_##inst, POST_KERNEL, 80, &api_my_driver);

DT_INST_FOREACH_STATUS_OKAY(DEV_INST)                 