#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Santhosh");
MODULE_DESCRIPTION("Platform devices with platform data");

struct my_pdata {
    int id;
    char label[20];
};

/* Device 0 data */
static struct my_pdata pdata0 = {
    .id = 5,
    .label = "SensorNode",
};

/* Device 1 data */
static struct my_pdata pdata1 = {
    .id = 10,
    .label = "ActuatorNode",
};

static struct platform_device dev0 = {
    .name = "myplatformdev",
    .id   = 0,
    .dev = {
        .platform_data = &pdata0,
    },
};

static struct platform_device dev1 = {
    .name = "myplatformdev",
    .id   = 1,
    .dev = {
        .platform_data = &pdata1,
    },
};

static int __init pdev_init(void)
{
    int ret;

    printk(KERN_INFO "[pdev] Registering device 0\n");
    ret = platform_device_register(&dev0);
    if (ret)
        return ret;

    printk(KERN_INFO "[pdev] Registering device 1\n");
    ret = platform_device_register(&dev1);
    if (ret) {
        platform_device_unregister(&dev0);
        return ret;
    }

    return 0;
}

static void __exit pdev_exit(void)
{
    platform_device_unregister(&dev1);
    platform_device_unregister(&dev0);

    printk(KERN_INFO "[pdev] Devices unregistered\n");
}

module_init(pdev_init);
module_exit(pdev_exit);
