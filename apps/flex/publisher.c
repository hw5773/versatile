#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <flex/flex.h>

int main(int argc, char *argv[])
{
	int len, err;
  unsigned char *fn;

  if (argc != 2)
  {
    APP_LOG("Need one argument representing the content to be inserted into the repository");
    APP_LOG1s("Usage: %s <file name>", argv[0]);
    exit(1);
  }

	APP_LOG("Start Flex ID Test Publisher Application");

  fn = argv[1];

  if ((err = init_flex()) < 0) goto out;

  APP_LOG1d("urepo_sock", urepo_sock);

  if ((err = pub(fn)) < 0) goto out;

  free_flex();

	return SUCCESS;

out:
  APP_LOG1d("Error", err);
  return err;
}
