#include <zephyr/shell/shell.h>
#include <zephyr/drivers/sensor.h>
#include "my_driver.h"

#define MIN_SLEEP_MS 50
#define MAX_SLEEP_MS 2500

static int cmd_channel_get_handler(const struct shell* sh, int argc, char** argv)
{
    shell_info(sh, "SUB CMD read / channel_get (Turn LED OFF)");

    const struct device *dev = shell_device_get_binding(argv[1]);

    if (!dev) {
        shell_error(sh, "Could not find device: %s !! ", argv[1]);
        return -EFAULT;
    }

    struct sensor_value val;
    int ret = sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &val);

    if (ret != 0)
    {
        shell_error(sh, "Could not get channel, ret: %d ", ret);
        return -EFAULT;
    }

    shell_info(sh, "result val: %d", val.val1);

    return 0;
}

static int cmd_sample_fetch_handler(const struct shell* sh, int argc, char** argv)
{
    shell_info(sh, "SUB CMD fetch / sample_fetch (Turn LED ON)");

    const struct device *dev = shell_device_get_binding(argv[1]);

    if (!dev) {
        shell_error(sh, "Could not find device: %s !! ", argv[1]);
        return -EFAULT;
    }

    int ret = sensor_sample_fetch(dev);

    if (ret != 0)
    {
        shell_error(sh, "Could not fetch channel, ret: %d ", ret);
        return -EFAULT;
    }

    return 0;
}

static int cmd_info_handler(const struct shell* sh, int argc, char** argv)
{
    shell_info(sh, "SUB CMD info (prints device name directly from hardware device tree)");

    const struct device *dev = shell_device_get_binding(argv[1]);

    if (!dev) {
        shell_error(sh, "Could not find device: %s !! ", argv[1]);
        return -EFAULT;
    }

    if (!device_is_ready(dev)) {
        shell_error(sh, "Device '%s' is not ready for use!", argv[1]);
        return -EBUSY;
    }

    shell_print(sh, "Successfully found device: %s", dev->name);

    return 0;
}

static int cmd_set_handler(const struct shell* sh, int argc, char** argv)
{
    shell_info(sh, "SUB CMD set <device name> <sleep value in ms>");

    const struct device *dev = shell_device_get_binding(argv[1]);

    /* TODO: validate the argument argv2 */

    if (!dev) {
        shell_error(sh, "Could not find device: %s !! ", argv[1]);
        return -EFAULT;
    }

    int err = 0;
    uint32_t sleep_ms_to_set;
    
    sleep_ms_to_set = shell_strtoul(argv[2], 10, &err);

    if (err != 0) {
        shell_error(sh, "Invalid value provided: %s", argv[2]);
        return -EINVAL;
    }

    if (sleep_ms_to_set < MIN_SLEEP_MS || sleep_ms_to_set > MAX_SLEEP_MS) {
        shell_error(sh, "Value %u is out of range! Expected bounds: [%d to %d] ms", 
                    sleep_ms_to_set, MIN_SLEEP_MS, MAX_SLEEP_MS);
        return -ERANGE; // Return standard embedded range out of bounds error
    }

    my_set_sleep_ms(dev, sleep_ms_to_set);
    shell_print(sh, "Sleep interval successfully updated to %u ms", sleep_ms_to_set);

    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(my_driver_subcmd,
    SHELL_CMD_ARG(read, NULL, "Get channel (Turn OFF LED) of my_driver. Expects: device name as arg", cmd_channel_get_handler, 2, 0),
    SHELL_CMD_ARG(fetch, NULL, "Sample fetch (Turn ON LED) of my_driver. Expects: device name as arg", cmd_sample_fetch_handler, 2, 0),
    SHELL_CMD_ARG(info, NULL, "Prints the device name of my_driver. Expects: device name as arg", cmd_info_handler, 2, 0), 
    SHELL_CMD_ARG(set, NULL, "Set the sleep value (in ms) for the led blinking. Expects: device name as arg1 and the value in ms as arg2", cmd_set_handler, 3, 0),
    SHELL_SUBCMD_SET_END,
);


SHELL_CMD_REGISTER(sensor, &my_driver_subcmd, "My driver set of cmds", NULL);