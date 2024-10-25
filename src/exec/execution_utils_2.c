/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artemii <artemii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 04:19:01 by mmychaly          #+#    #+#             */
/*   Updated: 2024/10/26 01:05:41 by artemii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*join_cmd_args(char **cmd_arg)
{
	int		i;
	char	*joined_args;
	char	*temp;
	size_t	total_length;

	total_length = 0;
	i = 1;
	if (!cmd_arg || !cmd_arg[0])
		return (NULL);
	while (cmd_arg[i])
	{
		total_length += ft_strlen(cmd_arg[i]) + 1;
		i++;
	}
	joined_args = malloc(total_length + 1);
	if (!joined_args)
		return (NULL);
	joined_args[0] = '\0';
	i = 1;
	while (cmd_arg[i])
	{
		if (ft_strlen(joined_args) > 0)
		{
			temp = joined_args;
			joined_args = ft_strjoin(joined_args, " ");
			free(temp);
		}
		temp = joined_args;
		joined_args = ft_strjoin(joined_args, cmd_arg[i]);
		free(temp);
		i++;
	}
	return (joined_args);
}

char	**join_arg(t_data *data)
{
	char	**temp;

	temp = malloc(sizeof(char *) * 3);
	if (temp == NULL)
		return (NULL);
	temp[0] = data->cmd[data->i]->cmd;
	temp[1] = join_cmd_args(data->cmd[data->i]->cmd_arg);
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
