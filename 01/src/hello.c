#include <linux/init.h>
#include <linux/module.h> 
#include <linux/kernel.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/device.h>

#define DEVICE_NAME "eudyptula" // Name that will appear in /dev
#define CLASS_NAME "eud" // Device class - character device driver


MODULE_LICENSE("GPL");
MODULE_AUTHOR("BENJI");
MODULE_DESCRIPTION("A simple Hello World module");

static int major_number; // Stores the device number - auto generated
static char message[256] = {0}; // Memory of string from user space
static short size_of_message; // Size of string
static int number_opens; // Number of times the device has been opened
static struct class *eud_char_class = NULL; // Device class struct pointer
static struct device *eud_char_device = NULL; // Device struct pointer

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

static int __init hello_init(void)
{
	pr_info("[*] Hello world!\n");

	// Try to dynamically generate a major number for device.
	major_number = register_chrdev(0, DEVICE_NAME, &f_ops);
	if (0 > major_number) {
		pr_warn("[!] Failed to register major number\n");
		return major_number;
	}
	pr_info("[*] Register major number: %d\n", major_number);

	// Register device class
	eud_char_class = class_create(THIS_MODULE, CLASS_NAME);
	if (IS_ERR(eud_char_class)) { // Check for error and clean up
		unregister_chrdev(major_number, DEVICE_NAME);
		pr_warn("[!] Failed to register device class\n");
		return PTR_ERR(eud_char_class); // Correct way to return an error on a pointer
	}
	
	pr_info("[*] Device class registered correctly.\n");

	// Register device driver
	eud_char_device = device_create(eud_char_class, NULL, 
		MKDEV(major_number, 0), NULL, DEVICE_NAME);
	if (IS_ERR(eud_char_device)) { // clean up if error
		class_destroy(eud_char_class);
		unregister_chrdev(major_number, DEVICE_NAME);
		pr_warn("[!] Failed to create device\n");
		return PTR_ERR(eud_char_device);
	}
	pr_info("[*] Eudyptula device created!\n");
	return 0;
}

static void __exit hello_cleanup(void)
{
	device_destroy(eud_char_class, MKDEV(major_number, 0));
	class_unregister(eud_char_class);
	class_destroy(eud_char_class);
	unregister_chrdev(major_number, DEVICE_NAME);
	pr_info("[*] Goodbye world!\n");
}

/*
 * The device open functioon that is called each time the device is opened.
 * This will increment the counter.
 */
static int dev_open(struct inode *inodep, struct file *filep)
{
	number_opens++;
	pr_info("[*] Eudyptula device opened %d times\n",number_opens);
	return 0;
}

/*
 * Called whenever the device is being fread from user space.
 */
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
	int error_count = 0;

	error_count = copy_to_user(buffer, message, size_of_message);

	if (0 == error_count) {
		pr_info("[*] Sent %d characters to user\n", size_of_message);
		return (size_of_message = 0);
	} else {
		pr_err("[!] Failed to send %d characters\n", size_of_message);
		return -EFAULT;
	}
}

/*
 * Called whenever device is written to from user space.
 */
static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
	sprintf(message, "%s(%zu letters)", buffer, len);
	size_of_message = strlen(message);
	pr_info("[*] Recieved %zu chars from user\n", len);
	return len;
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
