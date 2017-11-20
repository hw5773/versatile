#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <flex/flex.h>
#include <flex/flex_log.h>

int main(int argc, char *argv[])
{
	int err;
	flexid_t *tid;
	struct sockaddr_flex target_id;
	char buf[256];
	int rc, len;
  unsigned long start, end;

	APP_LOG("Start Flex ID Test Subscriber Application");

  // query message should be trasferred to the below layer. (in the get/put)
  if ((err = test_query(&tid)) < 0)
    goto out;

  APP_LOG("Set the test Flex ID Complete");

  APP_LOG("Invoke get()");

  start = get_current_microseconds();
  if ((err = get(tid, buf, &len)) < 0)
    goto out;
  end = get_current_microseconds();

  APP_LOG("Invoke get() success");
  APP_LOG1lu("Estimated Time", end - start);

	return SUCCESS;

out:
  APP_LOG1d("Error", err);
  return err;
}
