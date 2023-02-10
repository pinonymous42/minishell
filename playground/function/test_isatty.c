#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int main(void)
{
	int fd;

	fd = open("/bin/cat", O_RDONLY);
	if (isatty(fd) == 1)
		printf("tty!!!\n");
	else
		printf("not tty!!!\n");
	return (0);
}