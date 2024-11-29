/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_exit_utils_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmychaly <mmychaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 04:07:52 by mmychaly          #+#    #+#             */
/*   Updated: 2024/11/29 06:22:52 by mmychaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	error_empty_cmd(t_data *data)
{
	write(2, "Error: Empty command\n", 21);
	if (data->flag_pipe > 0)
		free_pipe(0);
	free_all_data(data);
	rl_clear_history();
	exit(127);
}

void	free_error_cmd(t_data *data)
{
	if (data->flag_pipe > 0)
		free_pipe(0);
	free_all_data(data);
	rl_clear_history();
	exit(127);
}

void	error_open_outfile(int flag, t_data *data)
{
	perror("open outfile");
	if (flag == 1)
		free_pipe(0);
	free_all_data(data);
	exit (1);
}

void	sigint_heredoc(t_data *data, int pipefd[2], int in)
{
	close(pipefd[1]);
	free_pipe(pipefd[0]);
	close(pipefd[0]);
	if (dup2(in, 0) == -1)
	{
		perror("Error dup2 in here doc");
		close(in);
		free_all_data(data);
		rl_clear_history();
		exit(1);
	}
	close(in);
	data->back_in_main = 1;
	data->exit_status = 130;
	g_sig = 0;
}
