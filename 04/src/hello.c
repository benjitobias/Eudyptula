#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("BENJI");
MODULE_DESCRIPTION("A simple Hello World module");

static int __init hello_init(void)
{
	printk(KERN_DEBUG "hello: Hello world!\n");
	return 0;
}

static void __exit hello_cleanup(void)
{
	printk(KERN_DEBUG "hello: Cleaning up module.\n");
}

module_init(hello_init);
module_exit(hello_cleanup);
