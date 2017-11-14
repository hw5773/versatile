#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <flex/flex.h>

int main(int argc, char *argv[])
{
	int sock;
	flexid_t *tid;
	struct sockaddr_flex target_id;
	char buf[256];
	int rc, len;

	char message[] = "{ \"interface\" : \"wire\", \"bandwidth\" : \"10 Mbps\" }";
	int message_len = strlen(message);

	APP_LOG("Start Flex ID Test Subscriber Application");

  tid = test_id();

  APP_LOG("Set the test Flex ID Complete");

  APP_LOG("Invoke get()");

  if (get(tid, buf, &len) < 0)
  {
    APP_LOG("Error in get()");
    exit(1);
  }

  APP_LOG("Invoke get() success");

	return 0;
}
