#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <flex/flex.h>
#include <flex/flex_tools.h>

int get_full_path(unsigned char *fn, unsigned char **path)
{
  int err, len;
  unsigned char *slash, term;

  err = -ERROR_MALLOC;
  (*path) = (unsigned char *)malloc(MAX_FILE_PATH_SIZE);
  if (!(*path)) goto out;

  slash = "/";
  term = 0;
  getcwd((*path), MAX_FILE_PATH_SIZE);
  APP_LOG1s("Current Working Directory", *path);
  len = strlen(*path);
  memcpy(*path + len, slash, strlen(slash));
  memcpy(*path + len + 1, fn, strlen(fn));
  memcpy(*path + len + 1 + strlen(fn), &term, sizeof(term));
  APP_LOG1s("Path", *path);

  return SUCCESS;

out:
  return err;
}
