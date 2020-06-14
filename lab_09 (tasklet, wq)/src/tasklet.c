#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/time.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("TASKLET1_MODULE");
MODULE_AUTHOR("Moxxx1e");

static struct timespec64 cur_time;
static char my_tasklet_data[] = "my_tasklet_function was called";
void my_tasklet_handler(unsigned long data);

DECLARE_TASKLET(my_tasklet, my_tasklet_handler,
    (unsigned long) & my_tasklet_data);

void my_tasklet_handler(unsigned long data)
{
    ktime_get_real_ts64(&cur_time);
    int h = cur_time.tv_sec / 3600 % 24;
    int m = cur_time.tv_sec / 60 % 60;
    int s = cur_time.tv_sec % 60;

    printk(KERN_INFO "TASKLET INFO:"
                     "[TIME:%.2d:%.2d:%.2d]\n"
                     "\nstate:  %ld, count: %d, data: %s\n",
        h, m, s, my_tasklet.state, my_tasklet.count, my_tasklet.data);
    return;
}

static const int def_irq = 1;
static int irq_counter = 0;
static irqreturn_t irq_handler(int irq, void* dev)
{
    if (irq == def_irq) {
        irq_counter++;
        printk(KERN_INFO "INTERRUPT! irq_counter = %d", irq_counter);
        tasklet_schedule(&my_tasklet);
        return IRQ_HANDLED; // прерывание обрабатано
    }
    return IRQ_NONE; // прерывание не обработано
}

static int dev_id;
static int __init tasklet1_module_init(void)
{
    /* Schedule the Bottom Half */
    if (request_irq(def_irq, irq_handler, IRQF_SHARED, "TASKLET_interrupt", &dev_id))
        return -1;

    printk(KERN_INFO "TASKLET1_MODULE loaded!");
    return 0;
}

static void __exit tasklet1_module_exit(void)
{
    /* Stop the tasklet before we exit */
    tasklet_kill(&my_tasklet);
    synchronize_irq(def_irq);
    free_irq(def_irq, &dev_id);
	printk(KERN_INFO "TASKLET1_MODULE: result irq_counter = %d\n", irq_counter);
    printk(KERN_INFO "TASKLET1_MODULE unloaded!\n");
}

module_init(tasklet1_module_init);
module_exit(tasklet1_module_exit);

