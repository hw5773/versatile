#ifndef __FLEX_LOG__
#define __FLEX_LOG__

#ifdef DEBUG
#define FLEX_LOG(msg) \
	printk(KERN_INFO "[Flex] %s: %s\n", __func__, msg)

#define FLEX_LOG1d(msg, arg1) \
	printk(KERN_INFO "[Flex] %s: %s: %d\n", __func__, msg, arg1)

#define FLEX_LOG1x(msg, arg1) \
	printk(KERN_INFO "[Flex] %s: %s: 0x%x\n", __func__, msg, arg1)

#define FLEX_LOG1s(msg, arg1) \
	printk(KERN_INFO "[Flex] %s: %s: %s\n", __func__, msg, arg1)
#else
#define FLEX_LOG(msg)
#define FLEX_LOG1d(msg, arg1)
#define FLEX_LOG1x(msg, arg1)
#define FLEX_LOG1s(msg, arg1)
#endif /* DEBUG */

#ifdef DEBUG
#define APP_LOG(msg) \
	printf("[Flex] %s: %s\n", __func__, msg)
#define APP_LOG1d(msg, arg1) \
  printf("[Flex] %s: %s: %d\n", __func__, msg, arg1)
#define APP_LOG1s(msg, arg1) \
  printf("[Flex] %s: %s: %s\n", __func__, msg, arg1)
#else
#define APP_LOG(msg)
#define APP_LOG1d(msg, arg1)
#endif /* DEBUG */

#endif /* __FLEX_LOG__ */
