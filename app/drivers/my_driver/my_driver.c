#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>

#define DT_DRV_COMPAT ram_driver

LOG_MODULE_REGISTER(sensor_ram_driver, LOG_LEVEL_INF);


struct my_driver_config {
    struct gpio_dt_spec led;
};


static int my_sample_fetch(const struct device *dev, enum sensor_channel chan) {
    const struct my_driver_config *cfg = dev->config;
    LOG_INF(" Turn LED ON *** \n");
    gpio_pin_set_dt(&cfg->led, 1);
    return 0;
}


static int my_get_channel(const struct device *dev, enum sensor_channel chan, struct sensor_value *val) {
    const struct my_driver_config *cfg = dev->config;
    LOG_INF(" Turn LED OFF --- \n");
    gpio_pin_set_dt(&cfg->led, 0);
    return 0;
}


static DEVICE_API(sensor, api_my_driver) = {
    .channel_get  = my_get_channel,
    .sample_fetch = my_sample_fetch,    
};

static int init(const struct device* dev) {
    const struct my_driver_config *cfg = dev->config;

    gpio_pin_configure_dt(&cfg->led, GPIO_OUTPUT_ACTIVE);    
    
    LOG_INF(" -- Device Initialized -- ");

    return 0;
}


// Automatically instantiate a distinct driver instance for every enabled 
// node matching this driver's compatible string in the devicetree.
#define DEV_INST(inst)                                 \
    static const struct my_driver_config my_driver_config_##inst = { \
        .led = GPIO_DT_SPEC_INST_GET(inst, gpios),     \
    };                                                 \
    DEVICE_DT_INST_DEFINE(inst, init, NULL, NULL,   \
                          &my_driver_config_##inst, POST_KERNEL, 80, &api_my_driver);

DT_INST_FOREACH_STATUS_OKAY(DEV_INST)                 