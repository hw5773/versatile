#ifndef __FLEX_LOG__
#define __FLEX_LOG__

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>

#ifdef DEBUG
int idx;
#define APP_LOG(msg) \
	printf("[Flex] %s: %s\n", __func__, msg)
#define APP_LOG1d(msg, arg1) \
  printf("[Flex] %s: %s: %d\n", __func__, msg, arg1)
#define APP_LOG1s(msg, arg1) \
  printf("[Flex] %s: %s: %s\n", __func__, msg, arg1)
#define APP_LOG1x(msg, arg1) \
  printf("[Flex] %s: %s: %x\n", __func__, msg, arg1)
#define APP_LOG1lu(msg, arg1) \
  printf("[Flex] %s: %s: %lu\n", __func__, msg, arg1)
#define APP_LOG1p(msg, arg1) \
  printf("[Flex] %s: %s: %p\n", __func__, msg, arg1)
#define APP_LOG2s(msg, arg1, arg2) \
  printf("[Flex] %s: %s: ", __func__, msg); for(idx=0;idx<arg2;idx++) printf("%02X ", arg1[idx]);printf("\n");
#define APP_LOG2c(msg, arg1, arg2) \
  printf("[Flex] %s: %s: ", __func__, msg); for(idx=0;idx<arg2;idx++) printf("%c", arg1[idx]);printf("\n");
#define APP_LOGid(msg, arg1, arg2) \
  printf("[Flex] %s: %s: ", __func__, msg); printf("%02X ", arg1->flag); for(idx=0;idx<arg2-1;idx++) printf("%02X ", arg1->identity[idx]);printf("\n");
#else
#define APP_LOG(msg)
#define APP_LOG1d(msg, arg1)
#define APP_LOG1lu(msg, arg1)
#define APP_LOG1p(msg, arg1)
#define APP_LOG2s(msg, arg1, arg2)
#define APP_LOG2c(msg, arg1, arg2)
#define APP_LOGid(msg, arg1, arg2)
#endif /* DEBUG */

unsigned long get_current_microseconds();

#endif /* __FLEX_LOG__ */
