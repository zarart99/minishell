/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artemii <artemii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 16:16:35 by azakharo          #+#    #+#             */
/*   Updated: 2024/10/13 22:15:46 by artemii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

extern char	**environ;

// static void cleanup(t_command **commands, char *input)
//{
//    // Освобождаем массив структур команд
//    free_parsed_commands(commands);

//    // Освобождаем введённую строку
//    free(input);
//}

void	free_commands(t_command *cmd)
{
	int	i;

	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	if (cmd->input_file)
		free(cmd->input_file);
	if (cmd->output_file)
		free(cmd->output_file);
	free(cmd);
}
void	print_commands(char **args)
{
	int	i;

	i = 0;
	while (args[i] != NULL)
	{
		printf("Command %s:\n", args[i]);
		i++;
	}
}

int	main(void)
{
	char	*user_input;
	char	**envp;

	envp = environ;
	t_command *commands; //  указатель на структуру команды
	while (1)
	{
		// Получаем ввод пользователя
		user_input = readline("minishell$ ");
		if (!user_input)
			break ;
		if (ft_strlen(user_input) > 0)
			add_history(user_input);
		// Парсим введённую строку и получаем структуру команды
		commands = parse_pipeline(user_input);
		if (commands)
		{
			print_commands(commands->args);
			execute_pipeline(commands, envp);
			free_commands(commands);
			// Функция для освобождения структуры команды
			free(user_input);
		}
	}
	rl_clear_history();
	return (0);
}
