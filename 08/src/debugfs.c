#include <linux/module.h> // included for all kernel modules
#include <linux/kernel.h> // included for KERN_DEBUG
#include <linux/init.h> // included for __init and __exit macros
#include <linux/debugfs.h> // included for debugfs
#include <linux/fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("BENJI");
MODULE_DESCRIPTION("Eudyptula 08 debugfs module");

#define EUD_ID "b9c2282a294c"
#define ID_LEN sizeof(EUD_ID)

// Directory entry point will point to '/sys/kernel/debug/eudyptula'
// File will point to '/sys/kernel/debug/eudyptula/id'
static struct dentry *dir;
static struct dentry *id;

static char *id_buf = EUD_ID;

int filevalue;

static ssize_t dev_read(struct file *fp, char __user *user_buffer, size_t count, loff_t *position)
{
	return simple_read_from_buffer(user_buffer, count, position, id_buf, ID_LEN);
}

static const struct file_operations f_ops = {
	.read = dev_read,
};

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

	junk = debugfs_create_file("id", 0666, dir, &filevalue, &f_ops);
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
