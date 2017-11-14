#include "types.h"

struct id_manage_ops
{
	int (*reg)(FlexID *id);
	int (*update)(FlexID *id, unsigned char *key, unsigned char *value);
	int (*request)(FlexID *id);
	int (*query)(Query *q, FlexIDList **reply);
}
