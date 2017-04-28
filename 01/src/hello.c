#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/usb/input.h>
#include <linux/hid.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("BENJI");
MODULE_DESCRIPTION("A simple Hello World module");

static struct usb_device_id hello_table[] = {
	{ USB_INTERFACE_INFO(
		USB_INTERFACE_CLASS_HID,
		USB_INTERFACE_SUBCLASS_BOOT,
		USB_INTERFACE_PROTOCOL_KEYBOARD) },
	{} //Terminating Entry
};

MODULE_DEVICE_TABLE(usb, hello_table);

static int __init hello_init(void)
{

	pr_info("[*] Hello world!\n");
	return 0;
}

static void __exit hello_cleanup(void)
{
	pr_info("[*] Goodbye world!\n");
}

module_init(hello_init);
module_exit(hello_cleanup);
