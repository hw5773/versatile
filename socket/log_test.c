#include <stdio.h>

#define LOG(msg) \
	printf("Test: %s\n", msg)

int main()
{
	LOG("Hello");

	return 0;
}
