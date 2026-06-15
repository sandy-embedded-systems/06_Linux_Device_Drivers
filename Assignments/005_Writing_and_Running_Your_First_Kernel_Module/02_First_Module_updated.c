#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/timekeeping.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Santhosh");

static int __init first_module_init(void)
{
	printk(KERN_INFO "Author : Santhosh\n");
	printk(KERN_INFO "Time: %lld\n", ktime_get_real_seconds());
    return 0;
}

static void __exit first_module_exit(void)
{
    printk(KERN_INFO "Goodbye, Kernel! Module Removed.\n");
}

module_init(first_module_init);
module_exit(first_module_exit);
