/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artemii <artemii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 16:16:35 by azakharo          #+#    #+#             */
/*   Updated: 2024/10/15 01:44:09 by artemii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

extern char	**environ;

void	print_command(t_command *command) //Просто для дебага
{
	int	j;

	ft_printf("Commands:\n");
	j = 0;
	while (command->args[j] != NULL) 
	{
		ft_printf("  Args[%d]: %s \n", j, command->args[j]);
		j++;
	}
	if (command->input_file)
		ft_printf("  Input file: %s\n", command->input_file);
	if (command->output_file)
		ft_printf("  Output file: %s\n", command->output_file);
	if (command->append)
		ft_printf("  Append mode: enabled\n");
	if (command->here_doc)
		ft_printf("  Here_doc: %s\n", command->here_doc);
	if (command->is_pipe)
		ft_printf("  Is pipe: enabled\n");
}

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

int	main(void)
{
	char		*user_input;
	char		**envp;
	t_command	*commands;

	envp = environ;
	while (1)
	{
		user_input = readline("minishell$ ");
		if (!user_input)
			break ;
		if (ft_strlen(user_input) > 0)
			add_history(user_input);
		// Парсим введённую строку и получаем структуру команды
		commands = parse_pipeline(user_input, envp);
		if (commands)
		{
			print_command(commands); // для дебага
			execute_pipeline(commands);
			free_commands(commands);
			// Функция для освобождения структуры команды
			free(user_input);
		}
	}
	rl_clear_history();
	return (0);
}
