#ifndef __FLEX_KLOG__
#define __FLEX_KLOG__

#ifdef KDEBUG
#define FLEX_LOG(msg) \
	printk(KERN_INFO "[Flex] %s: %s\n", __func__, msg)

#define FLEX_LOG1d(msg, arg1) \
	printk(KERN_INFO "[Flex] %s: %s: %d\n", __func__, msg, arg1)

#define FLEX_LOG1x(msg, arg1) \
	printk(KERN_INFO "[Flex] %s: %s: 0x%x\n", __func__, msg, arg1)

#define FLEX_LOG1s(msg, arg1) \
	printk(KERN_INFO "[Flex] %s: %s: %s\n", __func__, msg, arg1)

#define FLEX_LOG1p(msg, arg1) \
	printk(KERN_INFO "[Flex] %s: %s: %p\n", __func__, msg, arg1)

#define FLEX_LOG1lu(msg, arg1) \
  printk(KERN_INFO "[Flex] %s: %s: %lu\n", __func__, msg, arg1)
#else
#define FLEX_LOG(msg)
#define FLEX_LOG1d(msg, arg1)
#define FLEX_LOG1x(msg, arg1)
#define FLEX_LOG1s(msg, arg1)
#define FLEX_LOG1p(msg, arg1)
#define FLEX_LOG1lu(msg, arg1)
#endif /* KDEBUG */

#endif
