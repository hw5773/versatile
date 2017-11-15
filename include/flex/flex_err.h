#ifndef __FLEX_ERR__
#define __FLEX_ERR__

// Error Code List (1XX: NO, 2XX: WRONG)
#define NO_DEV               100
#define WRONG_MESSAGE_TYPE  200
#define WRONG_HEADER        201

void error_handling(char *);

#endif /* __FLEX_ERR__ */
