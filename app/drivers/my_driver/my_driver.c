#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#define DT_DRV_COMPAT my_driver

LOG_MODULE_REGISTER(my_driver, LOG_LEVEL_INF);

static int channel_get_impl (const struct device *dev,
                             enum sensor_channel chan,
                             struct sensor_value *val)
{
    LOG_INF("** Hello channel get - channel: %d", chan);
    return 0;
}

static DEVICE_API(sensor, api_my_driver) = {
    .channel_get = channel_get_impl,
};

static int init(const struct device* dev) {
    LOG_INF(" -- Device Initialized -- ");

    return 0;
}

DEVICE_DT_INST_DEFINE(0, init, NULL, NULL, NULL, POST_KERNEL, 80, &api_my_driver);