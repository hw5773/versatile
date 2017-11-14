#ifndef __FLEX__
#define __FLEX__

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
#include <flex/flex_socket.h>
#include <flex/flex_types.h>

flexid_t *test_id(void);
int get(struct flexid *id, char *resp, int *len);
int put(struct flexid *id, char *resp, int *len);
void error_handling(char *buf);

#endif /* __FLEX__ */
