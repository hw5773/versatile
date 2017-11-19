#ifndef __FLEX__
#define __FLEX__

#include <linux/types.h>

#define PROTOCOL_AUTHOR	"Hyunwoo Lee <hwlee2014@mmlab.snu.ac.kr>, Hyeonmin Lee <hmlee@mmlab.snu.ac.kr>, Dongjun Lee <djlee@mmlab.snu.ac.kr>, Hyunchul Oh <hcoh@mmlab.snu.ac.kr>"
#define PROTOCOL_DESC	"Flex Protocol"

#include <flex/flex_const.h>
#include <flex/flex_id.h>
#include <flex/flex_socket.h>
#include <flex/flex_types.h>
#include <flex/flex_err.h>
#include <flex/flex_log.h>

// Behaviors for the Applications
int init_repo(void);
int get(struct flexid *id, char *resp, int *len);
int put(struct flexid *id, char *resp, int *len);
int pub(unsigned char *fn);
int serv(unsigned char *crt, unsigned char *key);
void error_handling(char *buf);

static int repo_sock = 0;

// For the test reason
#define FIRST   0x00
#define SECOND  0x0c
#define THIRD   0x29
#define FOURTH  0xcb
#define FIFTH   0x8d
#define SIXTH   0x25

int test_sid(flexid_t **id);
int test_query(flexid_t **id);
int test_request(flexid_t *id, response_t **resp);

#endif /* __FLEX__ */
