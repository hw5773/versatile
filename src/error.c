#include <flex/flex_err.h>
#include <stdio.h>

void error_handling(char *buf)
{
  fputs(buf, stderr);
  fputc('\n', stderr);
  exit(1);
}
