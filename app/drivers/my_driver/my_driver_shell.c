#include <zephyr/shell/shell.h>
#include <zephyr/drivers/sensor.h>

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


SHELL_STATIC_SUBCMD_SET_CREATE(my_driver_subcmd,
    SHELL_CMD_ARG(read, NULL, "Get channel (Turn OFF LED) of my_driver. Expects: device name as arg", cmd_channel_get_handler, 2, 0),
    SHELL_CMD_ARG(fetch, NULL, "Sample fetch (Turn ON LED) of my_driver. Expects: device name as arg", cmd_sample_fetch_handler, 2, 0),
    SHELL_CMD_ARG(info, NULL, "Prints the device name of my_driver. Expects: device name as arg", cmd_info_handler, 2, 0), 
    SHELL_SUBCMD_SET_END,
);


SHELL_CMD_REGISTER(sensor, &my_driver_subcmd, "My driver set of cmds", NULL);