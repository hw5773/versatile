#include <stdlib.h>
#include <flex/flex_id.h>
#include <flex/flex_types.h>
#include <flex/id_manage.h>

struct flexid *test_id()
{
  flexid_t *id;
  init_flexid(&id);
  return id;
}

int init_flexid(flexid_t **id)
{
  (*id) = (flexid_t *)malloc(sizeof(flexid_t));

  return 1;
}
