#include "proto_flex.h"
#include "id_manage.h"

struct flexid *test_id()
{
  struct flexid *id;
  init_flexid(&id);
  return id;
} 

int init_flexid(struct flexid **id)
{
  (*id) = (struct flexid *)malloc(struct flexid);

  return 1;
}
