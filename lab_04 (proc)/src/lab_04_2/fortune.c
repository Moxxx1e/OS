#include "linux/module.h"
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <asm/uaccess.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Fortune cookie Kernel Module");
MODULE_AUTHOR("Kolganov Oleg, IU7-62B");

ssize_t fortune_read(struct file *filp, char* __user buf, size_t count, loff_t *f_pos);
ssize_t fortune_write(struct file *filp, const char* __user buf, size_t len, loff_t *f_pos);
int fortune_init(void);
void fortune_exit(void);

struct file_operations fops = {
    .owner = THIS_MODULE,
	.read = fortune_read,
	.write = fortune_write,
};

#define MAX_COOKIE_LENGTH PAGE_SIZE

static struct proc_dir_entry* proc_entry;
static char* cookie_pot;
static int cookie_index_write;
static int cookie_index_read;

ssize_t fortune_read(struct file *filp, char* __user buff, size_t count, loff_t *f_pos)
{
    int len;

    if (!cookie_index_write || *f_pos > 0)
        return 0;
    
    if (cookie_index_read >= cookie_index_write)
        cookie_index_read = 0;

    len = sprintf(buff, "%s\n", &cookie_pot[cookie_index_read]);
    cookie_index_read += len;
    *f_pos += len;

    return len;
}

ssize_t fortune_write(struct file* filp, const char* __user buff, size_t len, loff_t* data)
{
    size_t space_available = (MAX_COOKIE_LENGTH - cookie_index_write) + 1;

    if (len > space_available) {
        printk(KERN_INFO "fortune: cookie pot is full!\n");
        return -ENOSPC;
    }

    if (copy_from_user(&cookie_pot[cookie_index_write], buff, len)) {
        return -EFAULT;
    }

    cookie_index_write += len;
    cookie_pot[cookie_index_write - 1] = '\0';

    return len;
}

#define FILE_NAME "fortune"
#define DIR_NAME "fortune_dir"
#define SYMLINK_NAME "fortune_l"
int init_fortune_module(void)
{
    int ret = 0;

    cookie_pot = (char*)vmalloc(MAX_COOKIE_LENGTH);
    if(!cookie_pot) {
        ret = -ENOMEM;
        return ret;
    }

    memset(cookie_pot, 0, MAX_COOKIE_LENGTH);
    proc_entry = proc_create(FILE_NAME, 0666, NULL, &fops);

    if (proc_entry == NULL) {
        ret = -ENOMEM;
        vfree(cookie_pot);
        printk(KERN_INFO "fortune: Couldn't create proc entry!\n");
        return ret;
    }

    cookie_index_write = 0;
    cookie_index_read = 0;

    proc_mkdir(DIR_NAME, NULL);
    proc_symlink(SYMLINK_NAME, NULL, "/proc/" FILE_NAME);

    printk(KERN_INFO "fortune: Module loaded.\n");
    return ret;
}

void cleenup_module(void)
{
    remove_proc_entry(SYMLINK_NAME, NULL);
    remove_proc_entry(FILE_NAME, NULL);
    remove_proc_entry(DIR_NAME, NULL);
    if (cookie_pot) {
        vfree(cookie_pot);
    }
    printk(KERN_INFO "fortune: Module unloaded.\n");
}

module_init(init_fortune_module);
module_exit(cleenup_module);
