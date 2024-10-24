/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmychaly <mmychaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 04:19:01 by mmychaly          #+#    #+#             */
/*   Updated: 2024/10/23 02:07:18 by mmychaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../include/minishell.h"

char **join_arg(t_data *data)
{
	char **temp;
	int	strs_nb;

	if (data->cmd[data->i]->cmd != NULL && data->cmd[data->i]->cmd_arg != NULL)
		strs_nb = 3;
	else if (data->cmd[data->i]->cmd != NULL && data->cmd[data->i]->cmd_arg == NULL)
		strs_nb = 2;
	else
		return (NULL);
	temp = malloc(sizeof(char *) * strs_nb);
	if (temp == NULL)
		return (NULL);
	temp[0] = ft_strdup(data->cmd[data->i]->cmd);
	if (temp[0] == NULL)
		return (free(temp), NULL);
	if (data->cmd[data->i]->cmd_arg != NULL)
	{
		temp[1] = ft_strdup(data->cmd[data->i]->cmd_arg);
		if (temp[1] == NULL)
			return (free(temp[0]), free(temp), NULL);
		temp[2] = NULL;
	}
	else
		temp[1] = NULL;

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


/*
	temp = malloc(sizeof(char *) * 3);
	if (temp == NULL)
		return (NULL);
	temp[0] = data->cmd[data->i]->cmd;
	temp[1] = data->cmd[data->i]->cmd_arg;
	temp[2] = NULL;
*/