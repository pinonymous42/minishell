#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

int main()
{
    char *line = NULL;

    while (1)
    {
        line = readline("minishell> ");
        if (line == NULL || strlen(line) == 0 || *line == EOF)
        {
            free(line);
            // break;
        }
        printf("line is '%s'\n", line);
        if (strlen(line) != 0)
        {
            add_history(line);
            free(line);
        }
    }
    printf("exit\n");
    return 0;
}