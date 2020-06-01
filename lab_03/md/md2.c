#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Moxxx1e");

extern const int hw_code;
extern char* hw_message;
extern const int gb_code;
extern char* gb_message;
extern char* def_message;

extern int factorial(int number);
extern char* switch_string(int number);
extern int return_zero(void);

static int __init my_module_init(void)
{
    printk(KERN_INFO "MD2: module loaded\n");

    printk(KERN_INFO "MD2: export variables:\n"
                     "hw_code: %d, hw_message: %s\n"
                     "gb_code: %d, gb_message: %s\n"
                     "def_message: %s\n",
        hw_code, hw_message,
        gb_code, gb_message, def_message);

    printk(KERN_INFO "MD2: export functions:\n"
                     "factorial(5): %d\n"
                     "return_zero(): %d\n"
                     "switch_string(1): %s",
        factorial(5), return_zero(), switch_string(1));

    return 0;
}

static void __exit my_module_exit(void)
{
    printk(KERN_INFO "MD2: module unloaded\n");
}

module_init(my_module_init);
module_exit(my_module_exit);
