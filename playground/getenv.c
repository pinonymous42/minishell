#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	printf("%s\n", getenv("TERM_PROGRAM"));
	return (0);
}