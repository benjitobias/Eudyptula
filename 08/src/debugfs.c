#include <linux/module.h> // included for all kernel modules
#include <linux/kernel.h> // included for KERN_DEBUG
#include <linux/init.h> // included for __init and __exit macros
#include <linux/debugfs.h> // included for debugfs

MODULE_LICENSE("GPL");
MODULE_AUTHOR("BENJI");
MODULE_DESCRIPTION("Eudyptula 08 debugfs module");

// Directory entry point will point to '/sys/kernel/debug/eudyptula'
static struct dentry *dir = 0;

// File 'sys/kernel/debug/eudyptula/id'
static u32 id = 0;

static int __init hello_init(void)
{	
	struct dentry *junk;
	printk(KERN_DEBUG "eudyptula 08: Loaded!\n");
	
	// create dir
	dir = debugfs_create_dir("eudyptula", 0);
	if (!dir) {
		printk(KERN_ALERT, "eudyptula 08: failed to create /sys/kernel/debug/eudyptula\n");
		return -1;
	}
	
	junk = debugfs_create_u32("id", 0666, dir, &id);
	if (!junk) {
		printk(KERN_ALERT, "eudyptula 08: failed to create /sys/kernel/debug/eudyptula/id\n");
		return -2;
	}
	
	return 0; // Non-zero return means that the module couldn't be loaded.
}

static void __exit hello_cleanup(void)
{
	printk(KERN_DEBUG "eudyptula 08: Cleaning up module.\n");
	debugfs_remove_recursive(dir);
}

module_init(hello_init);
module_exit(hello_cleanup);
