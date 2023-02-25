#include <stdio.h>
#include "./includes/step_1_4.h"

int main(int argc, char **argv, char **envp)
{
    printf("%d\n", access(NULL, X_OK));
    return (0);
}