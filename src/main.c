/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artemii <artemii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 16:16:35 by azakharo          #+#    #+#             */
/*   Updated: 2024/10/10 01:04:54 by artemii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

extern char	**environ;

static void cleanup(t_command **commands, char *input)
{
    // Освобождаем массив структур команд
    free_parsed_commands(commands);

    // Освобождаем введённую строку
    free(input);
}
void	print_commands(char ***commands)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (commands[i] != NULL)
	{
		printf("Command %d:\n", i);
		j = 0;
		while (commands[i][j] != NULL)
		{
			printf("  Arg %d: %s\n", j, commands[i][j]);
			j++;
		}
		i++;
	}
}

int	main(void)
{
	char		*input;
	t_command	**commands;
	char		**envp;
	int			num_cmds;

	envp = environ;
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
			break ;
		if (ft_strlen(input) > 0)
			add_history(input);
		commands = parse_pipeline(input);
		num_cmds = 0;
		while (commands[num_cmds] != NULL)
			num_cmds++;
		// Вызываем execute_pipeline с количеством команд
		if (commands)
			execute_pipeline(commands, envp, num_cmds);
		cleanup(commands, input);
	}
	rl_clear_history();
	return (0);
}
