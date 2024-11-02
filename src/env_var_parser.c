/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artemii <artemii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 00:06:31 by artemii           #+#    #+#             */
/*   Updated: 2024/11/02 19:18:29 by artemii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*extract_var_name(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '$')
		i++;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_')) // Считаем, пока не встретится недопустимый символ
		i++;
	return (ft_substr(str, 1, i - 1)); // Возвращаем имя переменной без '$'
}

// Проверка на существование переменной в окружении (без получения её значения)
int	env_var_exists(const char *name)
{
	extern char	**environ;
	int			len;

	len = strlen(name);
	for (int i = 0; environ[i] != NULL; i++)
	{
		if (strncmp(environ[i], name, len) == 0 && environ[i][len] == '=')
			return (1); // Переменная существует
	}
	return (0); // Переменная не найдена
}

char	*replace_substring(char *str, int start, int end,
		const char *replacement)
{
	char	*new_str;
	int		new_len;

	new_len = ft_strlen(str) - (end - start) + ft_strlen(replacement);
	new_str = malloc(new_len + 1);
	if (!new_str)
		return (NULL);
	ft_strlcpy(new_str, str, start + 1); // Копируем часть строки до начала замены
	if (replacement != NULL)
		ft_strlcat(new_str, replacement, new_len + 1);
	ft_strlcat(new_str, str + end, new_len + 1); // Добавляем оставшуюся часть строки после замены
	free(str);                                  
	return (new_str);
}

char	*process_variable(char *result, int *i)
{
	char	*var_name;
	char	*var_value;
	int		end;

	var_name = extract_var_name(result + *i); //Извлекаем переменную
	if (var_name)
	{
		if (env_var_exists(var_name)) // Проверяем существование переменной
		{
			var_value = getenv(var_name);
			if (var_value == NULL)
				var_value = ""; // Если переменная существует, но значение пустое
			result = replace_substring(result, *i, *i + ft_strlen(var_name) + 1,
					var_value);
			*i += ft_strlen(var_value); // Сдвигаем индекс на длину значения
		}
		else
		{
			end = *i + ft_strlen(var_name) + 1; // Если переменной нет,просто удаляем её из строки
			ft_memmove(result + *i, result + end, ft_strlen(result + end) + 1);
		}
		free(var_name);
	}
	return (result);
}

char	*replace_env_var(char *input) // Функция,которая заменяет переменные окружения на их значения
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
