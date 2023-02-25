/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 23:57:42 by yokitaga          #+#    #+#             */
/*   Updated: 2023/02/25 23:35:36 by kohmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "./includes/step_1_4.h"

void	append_char(char **s, char c)
{
	size_t	size;
	char	*new;

	size = 2;
	if (*s)
		size += strlen(*s);
	new = malloc(size);
	if (new == NULL)
		fatal_error("malloc");
	if (*s)
		strlcpy(new, *s, size);
	new[size - 2] = c;
	new[size - 1] = '\0';
	if (*s)
		free(*s);
	*s = new;
}

// bool    is_identifier(const char *str)
// {
//     if (ft_isalpha(*str) == 0 && *str != '_')
//         return (false);
//     str++;
//     while (*str)
//     {
//         if (ft_isalnum(*str) == 0 && *str != '_')
//             return (false);
//         str++;
//     }
//     return (true);
// }

char *ft_strndup(char *str, int n)
{
    char *ret;
    int i;

    ret = malloc(sizeof(char) * (n + 1));
    if (!ret)
        return (NULL);
    i = 0;
    while (n >= 0)
    {
        ret[i] = *str;
        i++;
        str++;
        n--;
    }
    ret[i] = '\0';
    return (ret);
}

char    *double_variable_expand(char *new_word, int stat_loc, t_environ *list)
{
    char    *tmp;
    char    *ret;
    char    *var;

    tmp = new_word;
    while (*new_word != '$')
        new_word++;
    if (new_word == tmp)
    {
        // printf("%s, %d\n", __FILE__, __LINE__);
        ret = ft_calloc(sizeof(char), 1);
    }
    else
        ret = ft_strndup(tmp, new_word - tmp - 1);
    new_word++;
    // printf("|%s|\n", ret);
    // exit(1);
    if (*new_word == '?')
    {
        ret = ft_strjoin_with_free(ret, ft_itoa(stat_loc), FIRST_PARAM);
        new_word++;
    }
    else
    {
        // printf("%s, %d\n", __FILE__, __LINE__);
        if (ft_strchr(new_word, ' '))
            var = ft_strndup(new_word, ft_strchr(new_word, ' ') - new_word - 1);
        else
            var = ft_strndup(new_word, ft_strchr(new_word, '\0') - new_word);
        // printf("%s, %d\n", __FILE__, __LINE__);
        // printf("|%s|\n", var);
        // if (ft_strchr(var, '\0'))
        //     printf("aaaaaa");
        // printf("%s\n", search_env(var, list));
        ret = ft_strjoin_with_free(ret, search_env(var, list), FIRST_PARAM);
        // printf("%s\n", ret);
        // exit(1);
        // printf("%s, %d\n", __FILE__, __LINE__);
        free(var);
        // printf("%s, %d\n", __FILE__, __LINE__);
        if (ft_strchr(new_word, ' '))
            new_word = ft_strchr(new_word, ' ');
        else
            new_word = ft_strchr(new_word, '\0');
    }
    ret = ft_strjoin_with_free(ret, new_word, FIRST_PARAM);
    // printf("[%s]\n", ret);
    free(tmp);
    // exit(1);
    return (ret);
    // new_word = ret;
    // printf("{%s}\n", new_word);
}

void    quote_removal(t_token *token, int stat_loc, t_environ *list)
{
    char *new_word;
    char *p;
    char *var;
    
    // printf("%s\n", token->word);
    // printf("%s, %d\n", __FILE__, __LINE__);
    if (token == NULL || (token->kind != TK_WORD && token->kind != TK_OP) || token->word == NULL)
    {
        // printf("%s, %d\n", __FILE__, __LINE__);
        return ;
    }
    p = token->word;
    new_word = NULL;
    // printf("%s, %d\n", __FILE__, __LINE__);
    while (*p != '\0')
    {
        if (*p == SINGLE_QUOTE)
        {
            //skip quote
            p++;
            while(*p != SINGLE_QUOTE)
            {
                if (*p == '\0')
                    assert_error("Unclosed single quote");
				append_char(&new_word, *p++);   
            }
            //skip quote
            p++;
        }
        else if (*p == DOUBLE_QUOTE)
        {
            //skip quote
            p++;
            while (*p != DOUBLE_QUOTE)
			{
				if (*p == '\0')
					assert_error("Unclosed double quote");
				append_char(&new_word, *p++);
            }
            //skip quote
            p++;
            // printf("%s\n", new_word);
            // exit(1);
            while (ft_strchr(new_word, '$'))
            {
                // printf("%s\n", new_word);
                // exit(1);
                // printf("%s, %d\n", __FILE__, __LINE__);
                new_word = double_variable_expand(new_word, stat_loc, list);
                // printf("%s\n", new_word);
                // exit(1);
                // printf("%s, %d\n", __FILE__, __LINE__);
            }
            // printf("|%s|\n", new_word);
        }
        else if (*p == '$')//$VAR="echo hello"と入っていても後でどうせ中身をsplitするからただgetenvだけでいいんじゃね
        {
            while (ft_strchr(p, '$'))
            {
                p++;
                if (*p == '?')
                {
                    p++;
                    // printf("%s, %d\n", __FILE__, __LINE__);
                    // printf("%d\n", stat_loc);
                    // printf("%s\n", ft_itoa(stat_loc));
                    new_word = ft_itoa(stat_loc);
                    // printf("%s, %d\n", __FILE__, __LINE__);
                }
                else
                {
                    // printf("%s, %d\n", __FILE__, __LINE__);
                    if (ft_strchr(p, '$'))
                        var = ft_strndup(p, ft_strchr(p, '$') - p - 1);
                    else
                        var = ft_strndup(p, ft_strchr(p, '\0') - p);
                    // printf("Z%sZ\n", var);
                    if (new_word == NULL)
                        new_word = ft_strdup(search_env(var, list));
                    else
                        new_word = ft_strjoin_with_free(new_word, search_env(var, list), FIRST_PARAM);
                    // printf("|%s|\n", new_word);
                    // printf("%s, %d\n", __FILE__, __LINE__);
                    free(var);
                    // printf("%s, %d\n", __FILE__, __LINE__);
                    if (ft_strchr(p, '$'))
                        p = ft_strchr(p, '$');
                    else
                        p = ft_strchr(p, '\0');
                }
                // printf("%s, %d\n", __FILE__, __LINE__);
            }
            // printf("%s, %d\n", __FILE__, __LINE__);
        }
        else
        {
            // printf("%s, %d\n", __FILE__, __LINE__);
          append_char(&new_word, *p++);  
        }
    }
    // printf("%s, %d\n", __FILE__, __LINE__);
    free(token->word);
    // printf("%s, %d\n", __FILE__, __LINE__);
    // printf("[%s]\n", new_word);
    token->word = new_word;
    // printf("%s, %d\n", __FILE__, __LINE__);
    // printf("[%s]\n", token->word);
    // printf("%s, %d\n", __FILE__, __LINE__);
    quote_removal(token->next, stat_loc, list);
}

void	expand_quote_removal(t_node *node, int stat_loc, t_environ *list)
{
	if (node == NULL)
		return ;
	quote_removal(node->args, stat_loc, list);
    // printf("%s, %d\n", __FILE__, __LINE__);
	expand_quote_removal(node->next, stat_loc, list);
}

void	expand(t_node *node, int stat_loc, t_environ *list)
{
	expand_quote_removal(node, stat_loc, list);
    // printf("%s, %d\n", __FILE__, __LINE__);
}