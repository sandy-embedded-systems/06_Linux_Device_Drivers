#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>

static int test_value = 10;
static char *author = "LinuxLearner";
static int repeat_count = 0;
static int arr[10];
int count =0;

module_param(test_value, int, 0660);     // int parameter
MODULE_PARM_DESC(test_value, "An integer value");

module_param(author, charp, 0660);       // char pointer (string) parameter
MODULE_PARM_DESC(author, "Author's name");

module_param(repeat_count, int, 0660);
MODULE_PARM_DESC(repeat_count,"an int val");

module_param_array(arr, int, &count, 0660);


static int __init param_module_init(void)
{
    int sum=0;
    printk(KERN_INFO "param_module loaded.\n");
    printk(KERN_INFO "test_value = %d\n", test_value);
    for(int i=0;i < repeat_count;i++){
	    printk(KERN_INFO "author = %s\n", author);
    }
    for(int i=0; i<count; i++)
    {
	    sum+=arr[i];
    }
    printk(KERN_INFO "Average=%d\n",sum/count);
    return 0;
}

static void __exit param_module_exit(void)
{
    printk(KERN_INFO "param_module removed.\n");
}

module_init(param_module_init);
module_exit(param_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Santhosh");
MODULE_DESCRIPTION("Kernel Module with Parameters");
