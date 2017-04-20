#include <linux/module.h> // included for all kernel modules
#include <linux/kernel.h> // included for KERN_DEBUG
#include <linux/init.h> // included for __init and __exit macros

MODULE_LICENSE("GPL");
MODULE_AUTHOR("BENJI");
MODULE_DESCRIPTION("A simple Hello World module");

static int __init hello_init(void)
{
    /*
        Initialize
    */
    printk(KERN_DEBUG "hello: Hello world!\n");
    return 0; // Non-zero return means that the module couldn't be loaded.
}

static void __exit hello_cleanup(void)
{
    /*
        Unload
    */
    printk(KERN_DEBUG "hello: Cleaning up module.\n");
}

module_init(hello_init);
module_exit(hello_cleanup);
