#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init_task.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Moxxx1e");

static int __init my_module_init(void) {
	printk(KERN_INFO "PS_MODULE: PS_MODULE loaded!\n");

	struct task_struct *task =  &init_task;
	do {
		printk(KERN_INFO "PS_MODULE: %s - %d, parent: %s - %d\n",
task->comm, task->pid, task->parent->comm, task->parent->pid);
	} while ((task = next_task(task)) != &init_task);
	
	printk(KERN_INFO "PS_MODULE: current: %s - %d, parent: %s - %d\n", current->comm, current->pid, current->parent->comm, current->parent->pid);

	return 0;
}

static void __exit my_module_exit(void) {
	printk(KERN_INFO "PS_MODULE: PS_MODULE unloaded!\n");
}

module_init(my_module_init);
module_exit(my_module_exit);
