/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artemii <artemii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 01:23:19 by artemii           #+#    #+#             */
/*   Updated: 2024/10/15 01:41:56 by artemii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_split(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	handle_redirections_and_pipes(t_command *command, char **tokens, int *i)
{
	if (ft_strcmp(tokens[*i], "<") == 0)
	{
		*i += 1;
		command->input_file = ft_strdup(tokens[*i]); // Сохраняем имя файла для ввода
	}
	else if (ft_strcmp(tokens[*i], ">") == 0) // Если найден оператор вывода
	{
		*i += 1;
		command->output_file = ft_strdup(tokens[*i]);
	}
	else if (ft_strcmp(tokens[*i], ">>") == 0)
	{
		*i += 1;
		command->output_file = ft_strdup(tokens[*i]);
		command->append = 1; // Устанавливаем флаг append(что нам надо обрабатывать в конец файла)
	}
	else if (ft_strcmp(tokens[*i], "<<") == 0) // Если найден оператор here_doc
	{
		*i += 1;
		command->here_doc = ft_strdup(tokens[*i]); // Кладём слово после << как LIM
	}
	else if (ft_strcmp(tokens[*i], "|") == 0)
		command->is_pipe = 1; // Устанавливаем флаг пайпа (наверное пригодится)
}

void	handle_arguments(t_command *cmd, char **tokens, int *i, int *arg_idx)
{
	cmd->args[*arg_idx] = ft_strdup(tokens[*i]); // Сохраняем аргумент
	*arg_idx += 1;
}

void	parse_input(t_command *command, char *input, int *cmd_idx)
{
	int		i;
	char	*tmp;
	char **tokens = ft_split_quotes(input); // Разбиваем строку на слова с учётом кавычек - все что в кавычках как одно слово

	i = 0;
	char *full_command = NULL; // Для объединения аргументов команды
	while (tokens[i] != NULL)
	{
		if (ft_strcmp(tokens[i], "<") == 0 || ft_strcmp(tokens[i], ">") == 0
			|| ft_strcmp(tokens[i], ">>") == 0 || ft_strcmp(tokens[i],
				"<<") == 0)
			handle_redirections_and_pipes(command, tokens, &i);
		else
		{
			if (full_command == NULL)
				full_command = ft_strdup(tokens[i]);
			else
			{
				tmp = full_command;
				full_command = ft_strjoin(full_command, " ");
				full_command = ft_strjoin(full_command, tokens[i]);
				free(tmp);
			}
		}
		i++;
	}
	command->args[*cmd_idx] = full_command;
	command->args[*cmd_idx + 1] = NULL; // Завершаем массив команд
	free_split(tokens);
}

// Разбор строк с пайпами и их аргументами
t_command	*parse_pipeline(char *input, char **envp)
{
	int	i;
	t_command *command = malloc(sizeof(t_command));	// Выделяем память для структуры команды
	char **command_tokens = ft_split(input, '|');  	// Разделяем строку на команды по пайпам (токен - это то что между пайпами тут)

	i = 0;
	ft_memset(command, 0, sizeof(t_command));
	command->envp = envp;
	command->args = malloc(sizeof(char *) * 100); // Выделяем память под массив команд
	while (command_tokens[i] != NULL) 	// Проходим по каждому токену команды
	{
		parse_input(command, command_tokens[i], &i); // Наполняем структуру командой
		i++;
	}
	command->args[i] = NULL;    // Завершаем массив команд
	free_split(command_tokens); // Освобождаем временный массив строк
	return (command);
}
