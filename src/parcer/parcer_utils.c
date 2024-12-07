/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcer_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artemii <artemii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 00:04:11 by artemii           #+#    #+#             */
/*   Updated: 2024/12/07 15:42:33 by artemii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*handle_variable(const char *str, int *i, t_data *data, char *result)
{
	int		start;
	char	*expanded;

	start = *i;
	(*i)++;
	if (str[*i] == '\0' || str[*i] == ' ')
		expanded = ft_strdup("$");
	else if (str[*i] == '$')
		return (expanded = handle_variable(str, i, data, result));
	else if (str[*i] == '\'' || str[*i] == '"')
		return (expanded = handle_quotes(str, i, data, result));
	else if (str[(*i)++] == '?')
		expanded = ft_itoa(data->exit_status);
	else
		expanded = extract_and_expand_variable(str, i, start, data);
	if (result == NULL)
		result = expanded;
	else
		result = merge_tokens(result, expanded);
	return (result);
}

char	*handle_backslash(const char *str, int *i, char *result)
{
	char	*escaped;

	(*i)++;
	if (str[*i])
		escaped = ft_strndup(str + (*i)++, 1);
	else
		escaped = ft_strdup("");
	if (result == NULL)
		result = escaped;
	else
		result = merge_tokens(result, escaped);
	return (result);
}

char	*handle_text(const char *str, int *i, char *result)
{
	int		start;
	char	*text;

	start = *i;
	while (str[*i] && !ft_strchr(" '$\"\\", str[*i]))
		(*i)++;
	text = ft_strndup(str + start, *i - start);
	if (result == NULL)
		result = text;
	else
		result = merge_tokens(result, text);
	return (result);
}

char	*process_token(const char *str, int *i, t_data *data)
{
	char	*result;

	result = NULL;
	while (str[*i] && str[*i] != ' ')
	{
		if (str[*i] == '\'' || str[*i] == '"')
			result = handle_quotes(str, i, data, result);
		else if (str[*i] == '$')
			result = handle_variable(str, i, data, result);
		else if (str[*i] == '\\')
			result = handle_backslash(str, i, result);
		else
			result = handle_text(str, i, result);
	}
	return (result);
}

char	**ft_split_quotes(const char *input, t_data *data)
{
	char	**tokens;
	int		i;
	int		token_count;

	i = 0;
	token_count = 0;
	if (!input || !data)
		return (NULL);
	tokens = malloc(sizeof(char *) * (ft_strlen(input) / 2 + 2));
	if (!tokens)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	while (input[i])
	{
		skip_spaces(input, &i);
		if (input[i])
			tokens[token_count++] = process_token(input, &i, data);
		skip_spaces(input, &i);
	}
	tokens[token_count] = NULL;
	return (tokens);
}
