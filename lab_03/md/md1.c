#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Moxxx1e");

extern int factorial(int number)
{
    int i, ans;

    printk(KERN_INFO "MD1: factorial function called!");
    ans = number;
    for (i = number - 1; i > 1; i--)
        ans *= i;

    return (ans == 0) ? (1) : (ans);
}
EXPORT_SYMBOL(factorial);

extern int return_zero(void)
{
    printk(KERN_INFO "MD1: return_zero function called!");
    return 0;
}
EXPORT_SYMBOL(return_zero);

const int hw_code = 1;
char* hw_message = "Hello, world!";
const int gb_code = 2;
char* gb_message = "Good by!";
char* def_message = "The quick brown fox jumps over the lazy dog.";

EXPORT_SYMBOL(hw_code);
EXPORT_SYMBOL(gb_code);
EXPORT_SYMBOL(hw_message);
EXPORT_SYMBOL(gb_message);
EXPORT_SYMBOL(def_message);

extern char* switch_string(int number)
{
    printk(KERN_INFO "MD1: switch_string function called!");
    switch (number) {
    case hw_code:
        return hw_message;
    case gb_code:
        return gb_message;
    default:
        return def_message;
    }
}
EXPORT_SYMBOL(switch_string);

static int __init my_module_init(void)
{
    printk(KERN_INFO "MD1: module loaded\n");
    return 0;
}

static void __exit my_module_exit(void)
{
    printk(KERN_INFO "MD1: module unloaded\n");
}

module_init(my_module_init);
module_exit(my_module_exit);
