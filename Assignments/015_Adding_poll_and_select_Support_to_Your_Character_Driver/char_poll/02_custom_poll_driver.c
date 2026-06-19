#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/poll.h>
#include <linux/wait.h>
#include <linux/mutex.h>
#include <linux/ioctl.h>

#define DEVICE_NAME "polldev"
#define CLASS_NAME  "pollclass"
#define BUF_LEN     256

/* IOCTL Commands */
#define POLL_IOC_MAGIC          'P'
#define IOCTL_SET_READ_READY    _IO(POLL_IOC_MAGIC, 1)
#define IOCTL_CLR_READ_READY    _IO(POLL_IOC_MAGIC, 2)
#define IOCTL_SET_CAN_WRITE     _IO(POLL_IOC_MAGIC, 3)
#define IOCTL_CLR_CAN_WRITE     _IO(POLL_IOC_MAGIC, 4)

static dev_t dev_num;
static struct cdev poll_cdev;
static struct class *poll_class;
static struct device *poll_device;

static char buffer[BUF_LEN];

/* Flags used by poll() */
static int read_ready = 0;
static int can_write  = 1;

static DECLARE_WAIT_QUEUE_HEAD(wq);
static DEFINE_MUTEX(lock);

/*-----------------------------------------------------------*/

static int dev_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "[%s] opened\n", DEVICE_NAME);
    return 0;
}

static int dev_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "[%s] closed\n", DEVICE_NAME);
    return 0;
}

/*-----------------------------------------------------------*/

static ssize_t dev_read(struct file *file,
                        char __user *buf,
                        size_t len,
                        loff_t *off)
{
    int ret;

    /* Block until data is marked ready */
    if (wait_event_interruptible(wq, read_ready))
        return -ERESTARTSYS;

    mutex_lock(&lock);

    ret = simple_read_from_buffer(buf,
                                  len,
                                  off,
                                  buffer,
                                  strlen(buffer));

    read_ready = 0;

    mutex_unlock(&lock);

    printk(KERN_INFO "[%s] read completed\n", DEVICE_NAME);

    return ret;
}

/*-----------------------------------------------------------*/

static ssize_t dev_write(struct file *file,
                         const char __user *buf,
                         size_t len,
                         loff_t *off)
{
    if (!can_write)
        return -EAGAIN;

    if (len > BUF_LEN - 1)
        len = BUF_LEN - 1;

    mutex_lock(&lock);

    if (copy_from_user(buffer, buf, len))
    {
        mutex_unlock(&lock);
        return -EFAULT;
    }

    buffer[len] = '\0';

    /* Data becomes readable */
    read_ready = 1;

    mutex_unlock(&lock);

    wake_up_interruptible(&wq);

    printk(KERN_INFO "[%s] wrote: %s\n",
           DEVICE_NAME,
           buffer);

    return len;
}

/*-----------------------------------------------------------*/

static long dev_ioctl(struct file *file,
                      unsigned int cmd,
                      unsigned long arg)
{
    switch (cmd)
    {
        case IOCTL_SET_READ_READY:
            read_ready = 1;
            wake_up_interruptible(&wq);
            printk(KERN_INFO "[%s] READ_READY set\n", DEVICE_NAME);
            break;

        case IOCTL_CLR_READ_READY:
            read_ready = 0;
            printk(KERN_INFO "[%s] READ_READY cleared\n", DEVICE_NAME);
            break;

        case IOCTL_SET_CAN_WRITE:
            can_write = 1;
            wake_up_interruptible(&wq);
            printk(KERN_INFO "[%s] CAN_WRITE set\n", DEVICE_NAME);
            break;

        case IOCTL_CLR_CAN_WRITE:
            can_write = 0;
            printk(KERN_INFO "[%s] CAN_WRITE cleared\n", DEVICE_NAME);
            break;

        default:
            return -EINVAL;
    }

    return 0;
}

/*-----------------------------------------------------------*/

static __poll_t dev_poll(struct file *file,
                         poll_table *wait)
{
    __poll_t mask = 0;

    /* Register current process with wait queue */
    poll_wait(file, &wq, wait);

    /* Device can be read */
    if (read_ready)
        mask |= POLLIN | POLLRDNORM;

    /* Device can be written */
    if (can_write)
        mask |= POLLOUT | POLLWRNORM;

    return mask;
}

/*-----------------------------------------------------------*/

static const struct file_operations fops = {
    .owner          = THIS_MODULE,
    .open           = dev_open,
    .release        = dev_release,
    .read           = dev_read,
    .write          = dev_write,
    .poll           = dev_poll,
    .unlocked_ioctl = dev_ioctl,
};

/*-----------------------------------------------------------*/

static int __init poll_driver_init(void)
{
    int ret;

    ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    if (ret)
        return ret;

    cdev_init(&poll_cdev, &fops);

    ret = cdev_add(&poll_cdev, dev_num, 1);
    if (ret)
    {
        unregister_chrdev_region(dev_num, 1);
        return ret;
    }

    poll_class = class_create(CLASS_NAME);
    if (IS_ERR(poll_class))
    {
        cdev_del(&poll_cdev);
        unregister_chrdev_region(dev_num, 1);
        return PTR_ERR(poll_class);
    }

    poll_device = device_create(poll_class,
                                NULL,
                                dev_num,
                                NULL,
                                DEVICE_NAME);

    if (IS_ERR(poll_device))
    {
        class_destroy(poll_class);
        cdev_del(&poll_cdev);
        unregister_chrdev_region(dev_num, 1);
        return PTR_ERR(poll_device);
    }

    printk(KERN_INFO "[%s] loaded\n", DEVICE_NAME);
    printk(KERN_INFO "[%s] major=%d minor=%d\n",
           DEVICE_NAME,
           MAJOR(dev_num),
           MINOR(dev_num));

    return 0;
}

/*-----------------------------------------------------------*/

static void __exit poll_driver_exit(void)
{
    device_destroy(poll_class, dev_num);
    class_destroy(poll_class);
    cdev_del(&poll_cdev);
    unregister_chrdev_region(dev_num, 1);

    printk(KERN_INFO "[%s] unloaded\n", DEVICE_NAME);
}

/*-----------------------------------------------------------*/

module_init(poll_driver_init);
module_exit(poll_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Santhosh");
MODULE_DESCRIPTION("Character Driver with POLLIN, POLLOUT and IOCTL controlled poll()");
