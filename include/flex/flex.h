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

/* Behaviors for the Applications */
int init_flex(void);
void free_flex(void);
int start_repo(void);
int get(flexid_t *id, char *resp, int *len);
int put(flexid_t *tid, flexid_t *sid, char *buf, int *len);
int pub(unsigned char *fn);
int serv(unsigned char *crt, unsigned char *key);
void error_handling(char *buf);

extern int urepo_sock;
extern struct hash_table urepo_table;

/* For the test definitions and functions */
/* publisher MAC */
#define FIRST1   0x00
#define SECOND1  0x0c
#define THIRD1   0x29
#define FOURTH1  0xcb
#define FIFTH1   0x8d
#define SIXTH1   0x25

/* subscriber MAC */
#define FIRST2   0x00
#define SECOND2  0x0c
#define THIRD2   0x29
#define FOURTH2  0x23
#define FIFTH2   0xaa
#define SIXTH2   0x2a

#define TEST_FILE_PATH "/home/ubuntu/versatile/apps/flex/test.txt"
#define MAX_FILE_PATH_SIZE 256
#define BUF_SIZE 1024

int test_pub(void);
int test_sid(flexid_t **id);
int test_query(flexid_t **id);
int test_request1(flexid_t *id, response_t **resp);
int test_request2(flexid_t *id, response_t **resp);

#endif /* __FLEX__ */
