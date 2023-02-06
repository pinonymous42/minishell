#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/param.h>

char *Getcwd (char *buf, size_t size)
{
	char *p = getcwd (buf, size);
	if (!p)
		perror("getcwd");
	return (p);
}

int main(int argc, char *argv[])
{

	char	path[MAXPATHLEN];

	if (argc == 2 && !strcmp(argv[1],"pwd"))
	{
		if (Getcwd (path, sizeof(path)) == NULL)
			exit (EXIT_FAILURE);
		puts(path);
	}

	exit (EXIT_SUCCESS);
}