#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Santhosh");

static int __init first_module_init(void)
{
	printk(KERN_INFO "Hello, Kernel! Module Loaded.\n");
    return 0;
}

static void __exit first_module_exit(void)
{
    printk(KERN_INFO "Goodbye, Kernel! Module Removed.\n");
}

module_init(first_module_init);
module_exit(first_module_exit);
