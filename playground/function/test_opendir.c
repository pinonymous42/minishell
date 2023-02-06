#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	DIR	*dir;
	struct dirent	*dp;
	char	path[512];

	if(argc <=  1){
			strcpy(path,".");
	}
	else{
		strcpy(path,argv[1]);
	}

	if((dir = opendir(path)) == NULL){
		perror("opendir");
		exit(-1);
	}

	for(dp = readdir(dir); dp != NULL; dp = readdir(dir)){
		printf("%s\n",dp->d_name);
	}
		closedir(dir);
}