I originally wrote a driver for it which I was told I didn't need to do.

To fix it, I cleaned up the code and compiled again.

I copied the module to /lib/modules/$(uname -r)/misc and ran depmod -a
I could then load it using "modprobe hello" (I kept running "modprobe hello.ko" and didn't understand why it wasn't working).

To make it load when plugging in the keyboard, I copied the module to /lib/modules/$(uname -r)/kernel/drivers/hid/usbhid and ran depmod again.

It loads when a USB device is plugged in but doesn't unload when disconnected.
