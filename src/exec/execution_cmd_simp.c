/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_cmd_simp.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmychaly <mmychaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 00:53:45 by mmychaly          #+#    #+#             */
/*   Updated: 2024/10/25 00:48:06 by mmychaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	execution_here_doc(t_data *data)
{
	char	*line;
	int		pipefd[2];

	line = NULL;
	if (pipe(pipefd) == -1)
		return ;
	line = get_next_line(0);
	while (line != NULL)
	{
		if ((ft_strncmp(data->cmd[data->i]->here_doc_file, line, ft_strlen(line) - 1)) == 0)
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, ft_strlen(line));
		free(line);
		line = get_next_line(0);
	}
	close(pipefd[1]);
	data->here_doc_pfd = pipefd[0];
}

void	redirection_input(t_data *data, int pipefd[2])
{
	if (data->cmd[data->i]->pos_here_doc > data->cmd[data->i]->pos_input)
		ft_redirection_here_doc(data, pipefd);
	else if (data->cmd[data->i]->pos_here_doc < data->cmd[data->i]->pos_input)
		ft_redirection_in(data, pipefd);
	else if (data->cmd[data->i]->here_doc_file == NULL && data->cmd[data->i]->input_file == NULL && data->prev_pipe != -1)
		ft_redirection_pipe(data, pipefd);
	else
	{
		if (data->i != data->nb_pipe)
    		close(pipefd[0]);
	}
}

void	redirection_output(t_data *data, int pipefd[2])
{
	if (data->cmd[data->i]->output_file != NULL || data->cmd[data->i]->append_file != NULL)
		ft_redirection_out_cmd(data, pipefd);
	else if (data->cmd[data->i]->output_file == NULL && data->cmd[data->i]->append_file == NULL && data->i != data->nb_pipe)
		ft_redirection_out_pipe(data, pipefd);
	else
	{
		if (data->i != data->nb_pipe)
    		close(pipefd[1]);
	}
}

void	ft_launch_cmd(t_data *data, int pipefd[2])
{
	char	**strs_argv;
	char	*cmd;

	redirection_input(data, pipefd);
	redirection_output(data, pipefd);
	if (data->cmd[data->i]->cmd_arg != NULL)//Вставь сюда вторую строку из массива аргументов
	{
		if (access(data->cmd[data->i]->cmd_arg, F_OK) == 0)  //тоже самое 
		{
			if (data->flag_pipe == 1)
				free_pipe(0);
       		if (access(data->cmd[data->i]->cmd_arg, R_OK) == -1)//тоже самое 
      	  	{
				write(2, "Error file in arg :Permission denied\n", 37);
				exit(1);
			}
   		}
	}
	if (data->cmd[data->i]->cmd[0] == '\0')
		error_empty_cmd(data);
	strs_argv = join_arg(data);//После внедрения масива строк для аргумента , убери эту функцию и указатель на указатели
	if (strs_argv == NULL)//Это тоже
		error_join_arg(data);//Тоже
	if (access(data->cmd[data->i]->cmd, F_OK | X_OK) == 0)
		cmd = ft_strdup(data->cmd[data->i]->cmd);
	else
		cmd = ft_envp_cherch(data->cmd[data->i]->cmd, data->envp);
	if (cmd == NULL)
		free_error_cmd(strs_argv, data);//Переделай функции убери strs_argv
	if (execve(cmd, strs_argv, data->envp) == -1)//Вместо strs_argv поставь масив для аргументов
		free_fault_execve(strs_argv, cmd, data);//Переделай функции убери strs_argv
}

void	execution_cmd(t_data *data)
{
	int	pid;
	int	pipefd[2];

	data->prev_pipe = -1;
	data->here_doc_pfd = -1;
	data->i = 0;
	data->flag_pipe = 0;
	while (data->i <= data->nb_pipe)
	{	
		if (data->cmd[data->i]->here_doc_file != NULL)
		{
			execution_here_doc(data);
			if (data->here_doc_pfd == -1)
				write(2, "ERROR in here_doc\n", 18);
		}
		if (data->i != data->nb_pipe && pipe(pipefd) == -1)
		{
			write(2, "ERROR: pipe",11);
			data->exit_status = 1;
			return ;
		}
		pid = fork();
		if (pid == -1)
		{
			write(2,"ERROR: fork", 11);
			data->exit_status = 1;
			return ;
		}
		if (pid == 0)
			ft_launch_cmd(data, pipefd);
		if (data->here_doc_pfd != -1 ) //Нужно ли высвобожать пайп в родительском если это уже сделанно в дочернем?
		{
			close(data->here_doc_pfd);
			data->here_doc_pfd = -1;
		}
		data->flag_pipe = 0;
		if (data->prev_pipe != -1)
			close(data->prev_pipe);
		if (data->i != data->nb_pipe)
			close(pipefd[1]);
		if (data->i == data->nb_pipe)
			data->prev_pipe = pid;
		else
			data->prev_pipe = pipefd[0];
		data->i++;
	}
	wait_processes(data);
}
