#include <flex/flex_log.h>
#include <flex/flex_err.h>
#include <flex/flex_const.h>
#include <flex/flex.h>
#include <flex/flex_types.h>

#include "flex_sock.h"
#include "flex_idtable.h"

struct id_table id_table __read_mostly;

int add_id_to_table(flexid_t *id)
{
  return SUCCESS;
}
