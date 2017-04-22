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

static int hello_probe(struct usb_interface *interface,
	const struct usb_device_id *id)
{
	pr_info("[*] Keyboard connected\n");
	return 0;
}

static void hello_disconnect(struct usb_interface *interface)
{
	pr_info("[*] Keyboard disconnected\n");
}


// usb driver
static struct usb_driver hello_driver = {
	.name = "Hello World Usb Driver",
	.id_table = hello_table, // usb_device_id
	.probe = hello_probe,
	.disconnect = hello_disconnect,
};

static int __init hello_init(void)
{
	int ret = -1;

	pr_info("[*] Hello world!");
	pr_info("[*] Registering driver");
	ret = usb_register(&hello_driver);
	pr_info("[*] Registration complete");
	return ret;
}

static void __exit hello_cleanup(void)
{
	pr_info("[*] Goodbye world!");
	usb_deregister(&hello_driver);
}

module_init(hello_init);
module_exit(hello_cleanup);
