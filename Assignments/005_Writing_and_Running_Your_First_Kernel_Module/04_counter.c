#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int counter;

static int __init counter_init(void)
{
	counter++;

	pr_info("counter: module loaded, count = %d\n", counter);

	return 0;
}

static void __exit counter_exit(void)
{
	pr_info("counter: module unloaded, final count = %d\n", counter);
}

module_init(counter_init);
module_exit(counter_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Santhosh");
MODULE_DESCRIPTION("Simple counter module");
