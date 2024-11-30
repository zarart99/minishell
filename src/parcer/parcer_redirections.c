/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcer_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artemii <artemii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 00:47:10 by artemii           #+#    #+#             */
/*   Updated: 2024/11/30 16:39:56 by artemii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**realloc_array(char **array, char *new_element)
{
	int		size;
	int		i;
	char	**new_array;

	size = 0;
	if (array)
		while (array[size])
			size++;
	new_array = malloc(sizeof(char *) * (size + 2));
	if (!new_array)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (i < size)
	{
		new_array[i] = array[i];
		i++;
	}
	new_array[size] = new_element;
	new_array[size + 1] = NULL;
	free(array);
	return (new_array);
}
int	is_redirection(const char *token)
{
	return (ft_strncmp(token, "<", 1) == 0 || ft_strncmp(token, ">", 1) == 0);
}

void	update_redirection(char **target_file, char ***file_array, char *token)
{
	if (*target_file)
		free(*target_file);
	*target_file = ft_strdup(token);
	*file_array = realloc_array(*file_array, ft_strdup(token));
}

int	has_redirection(const char *token)
{
	return (ft_strchr(token, '<') != NULL || ft_strchr(token, '>') != NULL);
}

// Разделяет токен на части: аргумент, редирекцию, файл
void	split_token_on_redirection(char *token, char **before, char **redir, char **after)
{
	int		i;

	*before = NULL;
	*redir = NULL;
	*after = NULL;

	if (!token || !*token) // Проверяем входные данные
		return;

	// Находим символ редирекции
	i = 0;
	while (token[i] && !ft_strchr("<>", token[i]))
		i++;

	// Разделяем строку
	if (token[i])
	{
		if (i > 0)
			*before = ft_strndup(token, i); // Все до редирекции
		*redir = ft_strndup(token + i, (token[i + 1] == '>' || token[i + 1] == '<') ? 2 : 1);
		*after = ft_strdup(token + i + ft_strlen(*redir)); // Может быть пустым
	}
	else
	{
		*before = ft_strdup(token); // Если редирекции нет, весь токен в before
	}
}





// Обрабатывает токен, содержащий редирекцию
void	process_redirection_token(t_cmd *cmd, char *token, int *redir_position, int *i, char **tokens)
{
	char	*before = NULL;
	char	*redir = NULL;
	char	*after = NULL;

	if (!token) // Проверяем на NULL
		return;

	split_token_on_redirection(token, &before, &redir, &after);

	// Если `after` пустой, берем следующий токен
	if (redir && (!after || *after == '\0'))
{
    if (tokens[*i + 1]) // Сначала проверяем, существует ли следующий токен
    {
        (*i)++; // Увеличиваем индекс только если следующий токен существует
        after = ft_strdup(tokens[*i]); // Копируем следующий токен
    }
    else
    {
        // Ошибка: нет файла после редирекции
        ft_printf("Error: missing file for redirection '%s'\n", redir);
        free(before);
        free(redir);
        exit(2); // Выходим, чтобы не обрабатывать NULL
    }
}


	// Обрабатываем редирекцию
	if (redir)
	{
		if (ft_strcmp(redir, "<") == 0)
		{
			cmd->pos_input = (*redir_position)++;
			update_redirection(&cmd->input_file, &cmd->input_files, after);
		}
		else if (ft_strcmp(redir, ">") == 0)
		{
			cmd->pos_output = (*redir_position)++;
			update_redirection(&cmd->output_file, &cmd->output_files, after);
		}
		else if (ft_strcmp(redir, ">>") == 0)
		{
			cmd->pos_append = (*redir_position)++;
			update_redirection(&cmd->append_file, &cmd->append_files, after);
		}
		else if (ft_strcmp(redir, "<<") == 0)
		{
			cmd->pos_here_doc = (*redir_position)++;
			update_redirection(&cmd->here_doc_file, &cmd->here_doc_files, after);
		}
	}

	// Добавляем "before" в аргументы, если существует
	if (before && *before != '\0')
	{
		handle_command_args(cmd, &before, NULL, redir_position); // Передаем before как аргумент
	}

	// Освобождаем временные строки
	free(before);
	free(redir);
	free(after);
}



void	handle_command_args(t_cmd *cmd, char **tokens, int *i, int *arg_idx)
{
	int	j;

	if (!cmd || !tokens || (i && !tokens[*i])) // Проверяем входные параметры
		return;

	if (cmd->cmd_arg == NULL)
	{
		cmd->cmd_arg = malloc(sizeof(char *) * 100);
		if (!cmd->cmd_arg)
			return;
		for (j = 0; j < 100; j++)
			cmd->cmd_arg[j] = NULL;

		cmd->cmd = ft_strdup(tokens[*i]);
		if (!cmd->cmd)
		{
			free(cmd->cmd_arg);
			cmd->cmd_arg = NULL;
			return;
		}
	}

	// Убедимся, что `tokens[*i]` валидный перед копированием
	if (i && tokens[*i])
		cmd->cmd_arg[(*arg_idx)++] = ft_strdup(tokens[*i]);
}





void	handle_here_docs(t_cmd *cmd, t_data *data)
{
	int	i;

	if (cmd->here_doc_file == NULL)
		return ;
	i = 0;
	while (cmd->here_doc_files[i] != NULL)
	{
		execution_here_doc(cmd, cmd->here_doc_files[i], data);
		if (data->back_in_main == 1)
			return ;
		i++;
	}
}
