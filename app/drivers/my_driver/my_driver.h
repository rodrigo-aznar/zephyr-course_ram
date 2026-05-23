#pragma MY_DRIVER_H

#include <zephyr/device.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint32_t my_get_sleep_ms(const struct device *dev);
void my_set_sleep_ms(const struct device *dev, uint32_t sleep_ms_to_set);

#ifdef __cplusplus
}
#endif