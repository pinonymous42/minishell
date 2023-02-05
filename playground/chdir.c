#include <stdio.h>
#include <unistd.h>
 
#define DATA_DIR "/bin"
#define PATH_SIZE 512
 
int main()
{
    char path_name[PATH_SIZE];

    getcwd(path_name, PATH_SIZE);
    printf("before current dir : %s\n", path_name);
 
    chdir(DATA_DIR);
    getcwd(path_name, PATH_SIZE);
    printf("after current dir : %s\n", path_name);
    return 0;
}