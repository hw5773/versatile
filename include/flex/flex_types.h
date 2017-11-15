#ifndef __FLEX_TYPES__
#define __FLEX_TYPES__

#include <flex/flex_hdr.h>
#include <flex/flex_id.h>
#include <flex/flex_request.h>

/* Flex Header Types */
typedef struct flexhdr      flexhdr_t;
typedef struct rflexhdr     rflexhdr_t;
typedef struct rflexhdr_ext rflexhdr_ext_t;
typedef struct uflexhdr     uflexhdr_t;
typedef struct uflexhdr_ext uflexhdr_ext_t;

/* Flex ID Types */
typedef struct flexid       flexid_t;
typedef struct flexid_ops   flexops_t;

/* Flex Control Types */
typedef struct response     response_t;
typedef struct query_list   query_t;
typedef struct reply_list   reply_t;

#endif /* __FLEX_TYPES__ */
