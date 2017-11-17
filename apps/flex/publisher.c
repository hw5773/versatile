#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <flex/flex.h>

int main(int argc, char *argv[])
{
	int err;
  unsigned char *fn;

	APP_LOG("Start Flex ID Test Publisher Application");

  if ((err = pub(fn)) < 0)
    goto out;

  APP_LOG1d("repo sock", repo_sock);

	return SUCCESS;

out:
  APP_LOG1d("Error", err);
  return err;
}
