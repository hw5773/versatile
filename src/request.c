#include <stdlib.h>

#include <flex/flex_types.h>
#include <flex/flex_request.h>
#include <flex/flex_err.h>
#include <flex/flex_log.h>

int init_response(response_t **resp)
{
  int err;
  (*resp) = (response_t *)malloc(sizeof(response_t));

  if (!(*resp))
  {
    APP_LOG("Error in malloc");
    err = -ERROR_MALLOC;
    goto out;
  }

  return SUCCESS;

out:
  return err;
}

int free_response(response_t *resp)
{
  free(resp);

  return SUCCESS;
}
