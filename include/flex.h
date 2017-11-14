#ifndef __FLEX_HEADER__
#define __FLEX_HEADER__
#endif

#include <linux/types.h>

#define PROTOCOL_AUTHOR	"Hyunwoo Lee <hwlee2014@mmlab.snu.ac.kr>, Hyeonmin Lee <hmlee@mmlab.snu.ac.kr>, Dongjun Lee <djlee@mmlab.snu.ac.kr>, Hyunchul Oh <hcoh@mmlab.snu.ac.kr>"
#define PROTOCOL_DESC	"Flex Protocol"

#ifdef DEBUG
#define APP_LOG(msg) \
	printf("[Flex] %s: %s\n", __func__, msg)
#else
#define APP_LOG(msg)
#endif

#include <flex/flex_const.h>
#include <flex/flex_id.h>

/* Flex socket address struct */
struct sockaddr_flex 
{
  __kernel_sa_family_t  sin_family;
  struct flexid         id;
  int                   message;
}

struct flex_id *test_id();
int get(struct flexid *id);
int put(struct flexid *id);
