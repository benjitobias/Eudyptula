#include <linux/init.h>
#include <linux/module.h> 
#include <linux/kernel.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/device.h>
#include <linux/miscdevice.h>

#define DEVICE_NAME "eudyptula" // Name that will appear in /dev
//#define CLASS_NAME "eud" // Device class - character device driver
#define EUD_ID "<ID>" // Eudyptula ID

MODULE_LICENSE("GPL");
MODULE_AUTHOR("BENJI");
MODULE_DESCRIPTION("A simple Hello World module");

//static int major_number; // Stores the device number - auto generated
static int number_opens; // Number of times the device has been opened
//static struct class *eud_char_class = NULL; // Device class struct pointer
//static struct device *eud_char_device = NULL; // Device struct pointer

static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

/*
 * Devices are represented as file structurs in the kernel. The file_operations
 * struct from /linux/fs.h lists the callback functions that you wish to
 * associate with the file operations.
 */
static struct file_operations f_ops =
{
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
	pr_info("[*] Loaded Eudyptula module.\n");
	if (!misc_register(&eud_device)) {
		pr_err("[!] Failed to register device\n");
		return -1;
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
	pr_info("[*] Eudyptula device opened %d times\n.",number_opens);
	return 0;
}

/*
 * Called whenever the device is being fread from user space.
 */
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
	int error_count = 0;
	unsigned int msg_len = 0;
	static char *msg = EUD_ID;
	
	msg_len = strlen(msg);
	
	error_count = copy_to_user(buffer, msg, msg_len);

	if (0 == error_count) {
		pr_info("[*] Sent %d characters to user.\n", msg_len);
		return (msg_len = 0);
	} else {
		pr_err("[!] Failed to send %d characters.\n", msg_len);
		return -EFAULT;
	}
}

/*
 * Called whenever device is written to from user space.
 */
static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
	if (!strcmp(buffer, EUD_ID)) {
		pr_info("[*] Recieved correct id: %s\n", buffer);
		return 0;
	} else {
		pr_warn("[!] Recieved bad id: %s\n", buffer);
		return -EINVAL;
	}
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
