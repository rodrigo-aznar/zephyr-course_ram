#include <zephyr/init.h>
#include <zephyr/kernel.h>

static int esp32_3248s035c_init(void)
{
    /* Use printk for early boot messages */
    printk(" *** Board ESP32-3248S035C initialized *** \n");

    return 0;
}

/* 
 * SYS_INIT arguments:
 * 1. Function to call
 * 2. Initialization level (PRE_KERNEL_1, PRE_KERNEL_2, POST_KERNEL, APPLICATION)
 * 3. Priority within that level (0 is highest)
 */
SYS_INIT(esp32_3248s035c_init, POST_KERNEL, 0);