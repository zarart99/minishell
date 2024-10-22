/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmychaly <mmychaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 04:19:01 by mmychaly          #+#    #+#             */
/*   Updated: 2024/10/22 04:39:19 by mmychaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../include/minishell.h"

char **join_arg(t_data *data)
{
	char **temp;

	temp = malloc(sizeof(char *) * 3);
	if (temp == NULL)
		return (NULL);
	temp[0] = data->cmd[data->i]->cmd;
	temp[1] = data->cmd[data->i]->cmd_arg;
	temp[2] = NULL;
	return (temp);
}

void	wait_processes(t_data *data)
{
	int	pid;
	int	status;

	pid = waitpid(-1, &status, 0);
	while (pid > 0)
	{
		if (pid == data->prev_pipe) //Здесь я неуверен
		{
			if (WIFEXITED(status))
				data->exit_status = (WEXITSTATUS(status));
			else
				data->exit_status = 1;
		}
		pid = waitpid(-1, &status, 0);
	}
}