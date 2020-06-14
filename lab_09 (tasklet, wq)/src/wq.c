#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/time.h>
 
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("WQ_MODULE");
MODULE_AUTHOR("Moxxx1e");
 
struct workqueue_struct *wq;
void workqueue_handler(struct work_struct *);
DECLARE_WORK(workname, workqueue_handler);
 
static struct timespec64 cur_time;
 
void workqueue_handler(struct work_struct* work) {
    ktime_get_real_ts64(&cur_time);
    int h = cur_time.tv_sec / 3600 % 24;
    int m = cur_time.tv_sec / 60 % 60;
    int s = cur_time.tv_sec % 60;
    printk(KERN_INFO "WORKQUEUE info: "
        "[TIME : %.2d:%.2d:%.2d]\n"
        "data: %d\n",
        h, m, s, work->data);
}

static const int def_irq = 1;
static int irq_counter = 0;
static irqreturn_t irq_handler(int irq, void* dev_id) {
    if (irq == def_irq) {
        irq_counter++;
        printk(KERN_INFO "Interrupt! irq_counter = %d\n", irq_counter);
        queue_work(wq, &workname);
        return IRQ_HANDLED; // прерывание обработано
    }
   
    return IRQ_NONE; // прерывание не обработано
}

static int dev_id;
static int __init wq_module_init(void) {
    if (request_irq(def_irq, irq_handler, IRQF_SHARED,
        "WorkQueue_interrupt", &dev_id)) {
        return -1;
    }
 
    wq = create_workqueue("wq");
    if (wq) {
        printk(KERN_INFO "WQ_MODULE: Workqueue created\n");
    }
 
    printk(KERN_INFO "WQ_MODULE loaded\n");
    return 0;
}
 
static void __exit wq_module_exit(void) {
    flush_workqueue(wq);
    destroy_workqueue(wq);
    synchronize_irq(def_irq);
    free_irq(def_irq, &dev_id);
    printk(KERN_INFO "WQ_MODULE: result irq_cnt = %d\n", irq_counter);
    printk(KERN_INFO "WQ_MODULE unloaded\n");
}
 
module_init(wq_module_init);
module_exit(wq_module_exit);
