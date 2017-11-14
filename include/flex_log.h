#define FLEX_LOG(msg) \
	printk(KERN_INFO "[Flex] %s: %s\n", __func__, msg)

#define FLEX_LOG1d(msg, arg1) \
	printk(KERN_INFO "[Flex] %s: %s: %d\n", __func__, msg, arg1)

#define FLEX_LOG1x(msg, arg1) \
	printk(KERN_INFO "[Flex] %s: %s: %x\n", __func__, msg, arg1)

#define FLEX_LOG1s(msg, arg1) \
	printk(KERN_INFO "[Flex] %s: %s: %s\n", __func__, msg, arg1)


