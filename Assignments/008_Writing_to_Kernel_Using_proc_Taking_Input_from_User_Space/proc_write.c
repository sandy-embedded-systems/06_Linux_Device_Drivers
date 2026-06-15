#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define CONTROL_PROC "control"
#define STATUS_PROC  "status"
#define MAX_LEN      100

static char command[MAX_LEN];
static char status[32] = "Stopped";

static ssize_t control_write(struct file *file,
                             const char __user *buf,
                             size_t count,
                             loff_t *ppos)
{
    if (count > MAX_LEN - 1)
        count = MAX_LEN - 1;

    if (copy_from_user(command, buf, count))
        return -EFAULT;

    command[count] = '\0';

    /* Remove trailing newline from echo */
    if (count > 0 && command[count - 1] == '\n')
        command[count - 1] = '\0';

    if (!strcmp(command, "Start"))
        strcpy(status, "Running");
    else if (!strcmp(command, "Stop"))
        strcpy(status, "Stopped");

    pr_info("Command received: %s\n", command);

    return count;
}

static ssize_t status_read(struct file *file,
                           char __user *buf,
                           size_t count,
                           loff_t *ppos)
{
    char temp[64];
    int len;

    len = snprintf(temp, sizeof(temp),
                   "Status: %s\n", status);

    return simple_read_from_buffer(buf, count,
                                   ppos, temp, len);
}

static const struct proc_ops control_fops = {
    .proc_write = control_write,
};

static const struct proc_ops status_fops = {
    .proc_read = status_read,
};

static int __init proc_demo_init(void)
{
    if (!proc_create(CONTROL_PROC, 0666, NULL, &control_fops))
        return -ENOMEM;

    if (!proc_create(STATUS_PROC, 0444, NULL, &status_fops)) {
        remove_proc_entry(CONTROL_PROC, NULL);
        return -ENOMEM;
    }

    pr_info("control and status proc entries created\n");
    return 0;
}

static void __exit proc_demo_exit(void)
{
    remove_proc_entry(CONTROL_PROC, NULL);
    remove_proc_entry(STATUS_PROC, NULL);

    pr_info("proc entries removed\n");
}

module_init(proc_demo_init);
module_exit(proc_demo_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Santhosh");
MODULE_DESCRIPTION("Control and Status Proc Demo");
