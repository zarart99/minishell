/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artemii <artemii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 16:16:35 by azakharo          #+#    #+#             */
/*   Updated: 2024/11/26 01:20:36 by artemii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int g_pid;

void print_commands(t_data *data)
{
    int i;
    int j;

    i = 0;
    while (data->cmd[i] != NULL)
    {
        ft_printf("Command[%d]:\n", i);

        if (data->cmd[i]->cmd)
            ft_printf("  cmd: %s\n",
                data->cmd[i]->cmd ? data->cmd[i]->cmd : "(null)");

        if (data->cmd[i]->cmd_arg)
        {
            j = 0;
            while (data->cmd[i]->cmd_arg[j] != NULL)
            {
                ft_printf("  cmd_arg[%d]: %s\n", j, data->cmd[i]->cmd_arg[j]);
                j++;
            }
        }
        else
            ft_printf("  cmd_arg: (null)\n");

        // Final input, output, append, and here_doc files
        if (data->cmd[i]->input_file)
            ft_printf("  final_input: %s\n", data->cmd[i]->input_file);
        else
            ft_printf("  final_input: (null)\n");

        if (data->cmd[i]->output_file)
            ft_printf("  final_output: %s\n", data->cmd[i]->output_file);
        else
            ft_printf("  final_output: (null)\n");

        if (data->cmd[i]->append_file)
            ft_printf("  final_append_file: %s\n", data->cmd[i]->append_file);
        else
            ft_printf("  final_append_file: (null)\n");

        if (data->cmd[i]->here_doc_file)
            ft_printf("  final_here_doc_file: %s\n", data->cmd[i]->here_doc_file);
        else
            ft_printf("  final_here_doc_file: (null)\n");

        // Print arrays of files
        if (data->cmd[i]->input_files)
        {
            j = 0;
            while (data->cmd[i]->input_files[j] != NULL)
            {
                ft_printf("  input_files[%d]: %s\n", j, data->cmd[i]->input_files[j]);
                j++;
            }
        }
        else
            ft_printf("  input_files: (null)\n");

        if (data->cmd[i]->output_files)
        {
            j = 0;
            while (data->cmd[i]->output_files[j] != NULL)
            {
                ft_printf("  output_files[%d]: %s\n", j, data->cmd[i]->output_files[j]);
                j++;
            }
        }
        else
            ft_printf("  output_files: (null)\n");

        if (data->cmd[i]->append_files)
        {
            j = 0;
            while (data->cmd[i]->append_files[j] != NULL)
            {
                ft_printf("  append_files[%d]: %s\n", j, data->cmd[i]->append_files[j]);
                j++;
            }
        }
        else
            ft_printf("  append_files: (null)\n");

        if (data->cmd[i]->here_doc_files)
        {
            j = 0;
            while (data->cmd[i]->here_doc_files[j] != NULL)
            {
                ft_printf("  here_doc_files[%d]: %s\n", j, data->cmd[i]->here_doc_files[j]);
                j++;
            }
        }
        else
            ft_printf("  here_doc_files: (null)\n");

        i++;
    }
}


char	**copy_envp(char **envp)
{
	int		i;
	int		count;
	char	**new_envp;

	count = 0;
	while (envp[count] != NULL)
		count++;
	new_envp = malloc(sizeof(char *) * (count + 1));
	if (!new_envp)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_envp[i] = ft_strdup(envp[i]); // Дублирую каждую строку
		if (!new_envp[i])
		{
			while (--i >= 0)
				free(new_envp[i]);
			free(new_envp);
			return (NULL);
		}
		i++;
	}
	new_envp[count] = NULL;
	return (new_envp);
}



int	main(int argc, char **argv, char **envp)
{
	t_data	*data;
	int		exit_status;

	(void)argv;
	if (argc > 2) //Проверка на количество аргументов
	{
		write(2, "Too many arguments\n", 19);
		return (1);
	}
	exit_status = 0;
	g_pid = -1; //-1 == родительский процесс
	signal(SIGINT, handle_sigint); //Функция которая обрабатывает сигнал ctrl c , во всех процессах
	signal(SIGQUIT, SIG_IGN); //Функция которая обрабатывает сигнал ctrl '\' . В родительском процессе , игнорирует сигнал
	data = malloc(sizeof(t_data));
	if (!data)
	{
		perror("malloc failed");
		return (1);
	}
	ft_memset(data, 0, sizeof(t_data));
	data->envp = copy_envp(envp); //Считываем окружение , нужно для execve
	    update_shlvl(data);

	while (1)
	{
		data->user_input = readline("minishell$ ");
		if (!data->user_input)
		{
			ft_printf("exit\n");
			break ;
		}
		if (ft_strlen(data->user_input) > 0)
			add_history(data->user_input);
		data->back_in_main = 0;
		data->builtin_cmd = 0;
		data->display_builtin_cmd = 0;
		if (g_pid == -50)
		{
			data->exit_status = 130;
    		exit_status = 130;
    		g_pid = -1;
		}
		parse_pipeline(data, data->user_input);
		if (data->back_in_main == 1)
		{
			exit_status = data->exit_status;
			free_data_cmd(data);
			free(data->user_input);
			continue ; // Пропускаем выполнение команды
		}
		data->exit_status = exit_status;
//		print_commands(data);    // Вывод команд для дебага
//		printf("\n---------\n"); //Отделяем вывод команды от дебага
		choice_execution(data);
//		printf("status %i\n", data->exit_status);
		exit_status = data->exit_status;
		free_data_cmd(data);
		free(data->user_input);
	}
	free_all_data(data);
	rl_clear_history();
	return (0);
}
