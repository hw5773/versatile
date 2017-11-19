#ifndef __FLEX_ERR__
#define __FLEX_ERR__

/* Error Code List (1XX: NO, 2XX: WRONG, 3XX: ERROR) */
#define NO_DEV              100
#define NO_RESP             101
#define NO_SOCK             102

#define WRONG_MESSAGE_TYPE  200
#define WRONG_HEADER        201

#define ERROR_GET           300
#define ERROR_SEND          301
#define ERROR_MALLOC        302
#define ERROR_READ_FILE     303
#define ERROR_WRITE_FILE    304
#define ERROR_INIT_MD       305
#define ERROR_INIT_MEM      306
#define ERROR_INIT_MD_CTX   307
#define ERROR_INIT_ID       308
#define ERROR_PROCESS_MD    309
#define ERROR_FINAL_MD      310
#define ERROR_MISTYPE       311
#define ERROR_BIND          312

void error_handling(char *);

#endif /* __FLEX_ERR__ */
