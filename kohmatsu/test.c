#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *ft_strndup(char *str, size_t n)
{
    char    *new_str;
    size_t  len;

    if (str == NULL)
        return (NULL);
    len = strlen(str);
    if (len > n)
        len = n;
    new_str = (char *)malloc(sizeof(char) * (len + 1));
    if (new_str == NULL)
        return (NULL);
    memcpy(new_str, str, len);
    new_str[len] = '\0';

    return (new_str);
}

int main(void)
{
    char *a = ft_strndup("abcdefg", 3);
    printf("%s\n", a);
    free(a);
    return (0);
}