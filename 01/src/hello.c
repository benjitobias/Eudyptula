#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>

#define DEVICE_NAME "eudyptula" // Name that will appear in /dev
#define EUD_ID "b9c2282a294c" // Eudyptula ID
#define ID_LEN sizeof(EUD_ID)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("BENJI");
MODULE_DESCRIPTION("A simple Hello World module");

static int number_opens; // Number of times the device has been opened
static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

/*
 * Devices are represented as file structurs in the kernel. The file_operations
 * struct from /linux/fs.h lists the callback functions that you wish to
 * associate with the file operations.
 */
static struct file_operations f_ops = {
	.open = dev_open,
	.read = dev_read,
	.write = dev_write,
	.release = dev_release,
};

struct miscdevice eud_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &f_ops,
};

static int __init hello_init(void)
{
	int error = 0;

	pr_info("[*] Loaded Eudyptula module.\n");
	error = misc_register(&eud_device);
	if (error) {
		pr_err("[!] Failed to register device\n");
		return error;
	}
	pr_info("[*] Got minor device number: %i", eud_device.minor);

	pr_info("[*] Eudyptula device created.\n");
	return 0;
}

static void __exit hello_cleanup(void)
{
	misc_deregister(&eud_device);
	pr_info("[*] Unloaded Eudyptula module.\n");
}

/*
 * The device open functioon that is called each time the device is opened.
 * This will increment the counter.
 */
static int dev_open(struct inode *inodep, struct file *filep)
{
	number_opens++;
	pr_info("[*] Eudyptula device opened %d times\n.", number_opens);
	return 0;
}

/*
 * Called whenever the device is being fread from user space.
 */
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
	return simple_read_from_buffer(buffer, len, offset, EUD_ID, sizeof(EUD_ID));
}

/*
 * Called whenever device is written to from user space.
 */
static ssize_t dev_write(struct file *filep, const char __user *buffer, size_t len, loff_t *offset)
{
	int val = 0;
	char *kbuffer;

	kbuffer = kzalloc(ID_LEN, sizeof(char));

	val = simple_write_to_buffer(kbuffer, ID_LEN, offset, buffer, ID_LEN);

	if (val > 0) {
		if (strncmp(kbuffer, EUD_ID, ID_LEN - 1)) {
			pr_warn("[!] Recieved bad id: %s\n", buffer);
			val = -EINVAL;	
			goto cleanup;
		}

	}

	pr_info("[*] Recieved correct id: %s\n", buffer);	

cleanup:
	kfree(kbuffer);
	return val;
}

/*
 * Called whenever device is closed/released.
 */
static int dev_release(struct inode *inodep, struct file *filep)
{
	pr_info("[*] Eudyptula device closed.\n");
	return 0;
}

module_init(hello_init);
module_exit(hello_cleanup);
