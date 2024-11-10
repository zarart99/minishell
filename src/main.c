/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artemii <artemii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 16:16:35 by azakharo          #+#    #+#             */
/*   Updated: 2024/11/10 19:17:40 by artemii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	print_commands(t_data *data)
{
	int	i;
	int	j;

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
		if (data->cmd[i]->input_file)
			ft_printf("  input_file: %s (pos_input: %d)\n",
				data->cmd[i]->input_file, data->cmd[i]->pos_input);
		else
			ft_printf("  input_file: (null)\n");
		if (data->cmd[i]->here_doc_file)
			ft_printf("  here_doc_file: %s (pos_here_doc: %d)\n",
				data->cmd[i]->here_doc_file, data->cmd[i]->pos_here_doc);
		else
			ft_printf("  here_doc_file: (null)\n");
		if (data->cmd[i]->output_file)
			ft_printf("  output_file: %s (pos_output: %d)\n",
				data->cmd[i]->output_file, data->cmd[i]->pos_output);
		else
			ft_printf("  output_file: (null)\n");
		if (data->cmd[i]->append_file)
			ft_printf("  append_file: %s (pos_append: %d)\n",
				data->cmd[i]->append_file, data->cmd[i]->pos_append);
		else
			ft_printf("  append_file: (null)\n");
		i++;
	}
}

//void init_global_envp(char **environ) {
//    int i = 0;

//    while (environ[i] != NULL && i < MAX_GLOBAL_ENV_SIZE - 1) {
//        global_envp[i] = strdup(environ[i]);
//        i++;
//    }
//    global_envp[i] = NULL;
//}

//void load_envp_into_data(t_data *data) {
//    int i = 0;

//    // Подсчитываем количество элементов в global_envp
//    while (global_envp[i] != NULL) {
//        i++;
//    }

//    // Освобождаем старое содержимое data->envp, если оно существует
//    if (data->envp != NULL) {
//        for (int j = 0; data->envp[j] != NULL; j++) {
//            free(data->envp[j]);
//        }
//        free(data->envp);
//    }

//    // Копируем значения из global_envp в data->envp
//    data->envp = malloc(sizeof(char *) * (i + 1));
//    if (!data->envp) {
//        perror("Failed to allocate data->envp");
//        exit(1);
//    }

//    for (int j = 0; j < i; j++) {
//        data->envp[j] = ft_strdup(global_envp[j]); // Копируем строки с выделением новой памяти
//    }
//    data->envp[i] = NULL;
//}

//void free_data_envp(t_data *data) {
//    if (data->envp) {
//        for (int i = 0; data->envp[i] != NULL; i++) {
//            free(data->envp[i]); // Освобождаем каждую строку
//        }
//        free(data->envp); // Освобождаем массив указателей
//        data->envp = NULL;
//    }
//}

//void free_global_envp() {
//    for (int i = 0; global_envp[i] != NULL; i++) {
//        free(global_envp[i]);
//    }
//}
// Копирование массива окружения envp в новый массив
char **copy_envp(char **envp)
{
	int		i;
	int		count;
	char	**new_envp;

	// Считаем количество переменных окружения
	count = 0;
	while (envp[count] != NULL)
		count++;

	// Выделяем память для массива указателей
	new_envp = malloc(sizeof(char *) * (count + 1));
	if (!new_envp)
		return (NULL);

	// Копируем каждую строку окружения в новый массив
	i = 0;
	while (i < count)
	{
		new_envp[i] = ft_strdup(envp[i]); // Дублируем каждую строку
		if (!new_envp[i])
		{
			// Освобождаем уже выделенные строки, если произошла ошибка
			while (--i >= 0)
				free(new_envp[i]);
			free(new_envp);
			return (NULL);
		}
		i++;
	}
	new_envp[count] = NULL; // Завершающий NULL
	return (new_envp);
}

void	free_data(t_data *data)
{
	int	i;
	int	j;

	// Освобождаем каждую команду в data->cmd
	i = 0;
	while (data->cmd[i] != NULL)
	{
		if (data->cmd[i]->cmd_arg)
		{
			j = 0;
			while (data->cmd[i]->cmd_arg[j])
			{
				free(data->cmd[i]->cmd_arg[j]);
				j++;
			}
			free(data->cmd[i]->cmd_arg);
		}
		if (data->cmd[i]->cmd)
			free(data->cmd[i]->cmd);
		if (data->cmd[i]->input_file)
			free(data->cmd[i]->input_file);
		if (data->cmd[i]->here_doc_file)
			free(data->cmd[i]->here_doc_file);
		if (data->cmd[i]->output_file)
			free(data->cmd[i]->output_file);
		if (data->cmd[i]->append_file)
			free(data->cmd[i]->append_file);
		free(data->cmd[i]);
		i++;
	}
		i = 0;
	//if(data->envp)
	//{ 
	//	while (data->envp[i] != NULL)
	//	{
	//		free(data->envp[i]);
	//		i++;
	//	}
	//}
	//free(data->envp);
	free(data->cmd);
	//free(data);
}

int	main(int argc, char **argv, char **envp)
{
	char	*user_input;
	t_data	*data;

	(void)argc;
	(void)argv;
		data = malloc(sizeof(t_data));
		if (!data)
		{
			perror("malloc failed");
			return (1);
		}
		ft_memset(data, 0, sizeof(t_data));
		data->envp = copy_envp(envp);
	 //Считываем окружение , нужно для execve
	while (1)
	{
		user_input = readline("minishell$ ");
		if (!user_input)
			break ;
		if (ft_strlen(user_input) > 0)
			add_history(user_input);
	

		
		parse_pipeline(data, user_input);
		print_commands(data);    // Вывод команд для дебага
		printf("\n---------\n"); //Отделяем вывод команды от дебага
		choice_execution(data);
		printf("status %i\n", data->exit_status);
		free_data(data);

		free(user_input);

		
	}
	int i=0;
	while (data->envp[i] != NULL)
		{
			free(data->envp[i]);
			i++;
		}
	free(data->envp);
	free(data);
	rl_clear_history();
	return (0);
}
