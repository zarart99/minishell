/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artemii <artemii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 00:06:31 by artemii           #+#    #+#             */
/*   Updated: 2024/10/25 00:53:08 by artemii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char *extract_var_name(const char *str)
{
    int i = 0;
    if (str[i] == '$')
        i++;
    while (str[i] && (ft_isalnum(str[i]) || str[i] == '_')) // Считаем, пока не встретится недопустимый символ
        i++;
    return (ft_substr(str, 1, i - 1)); // Возвращаем имя переменной без '$'
}

char *replace_substring(char *str, int start, int end, const char *replacement)
{
    char *new_str;
    int new_len = ft_strlen(str) - (end - start) + ft_strlen(replacement);
    
    new_str = malloc(new_len + 1);
    if (!new_str)
        return NULL;
    ft_strlcpy(new_str, str, start + 1);  // Копируем часть строки до начала замены
    ft_strlcat(new_str, replacement, new_len + 1);  // Добавляем замену
    ft_strlcat(new_str, str + end, new_len + 1);  // Добавляем оставшуюся часть строки после замены
    free(str);  // Освобождаем оригинальную строку
    return new_str;
}

char *process_variable(char *result, int *i)
{
    char *var_name;
    char *var_value;

    var_name = extract_var_name(result + *i); //Извлекаем переменную
    if (var_name)
    {
        var_value = getenv(var_name); // Получаем значения переменной
        if (var_value)
        {
            result = replace_substring(result, *i, *i + ft_strlen(var_name) + 1, var_value); // Заменяем переменную её значением из envp
            *i += ft_strlen(var_value);
        }
        else
            *i += ft_strlen(var_name) + 1;
        free(var_name);
    }
    return (result);
}

char *replace_env_var(char *input)
{
    char *result;
    int i;

    i = 0;
    result = ft_strdup(input);
    if (!result)
        return (NULL);
    while (result[i] != '\0')
    {
        if (result[i] == '$')
            result = process_variable(result, &i);
        else
            i++;
    }
    return (result);
}



