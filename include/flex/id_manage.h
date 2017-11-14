#ifndef __ID_MANAGE__
#define __ID_MANAGE__

#include <flex/flex_types.h>

struct id_manage_ops
{
	int (*reg)(flexid_t *id);
	int (*update)(flexid_t *id, unsigned char *key, unsigned char *value);
	int (*request)(flexid_t *id);
//	int (*query)(query_t *q, flexid_list_t **reply);
};

#endif /* __ID_MANAGE__ */
