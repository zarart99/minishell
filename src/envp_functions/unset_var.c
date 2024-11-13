/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_var.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmychaly <mmychaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 00:06:56 by artemii           #+#    #+#             */
/*   Updated: 2024/11/13 09:19:14 by mmychaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Удаление переменной из окружения
void	unset_var(t_data *data, char *key)
{
	int		key_len;
	int		i;

	key_len = ft_strlen(key);
	i = 0;
	if (data->nb_pipe != 0)//Закрываем input для составных команд
		close_input(data);
	while (data->envp[i] != NULL)
	{
		if (ft_strncmp(data->envp[i], key, key_len) == 0 // Совпадает ли текущий элемент envp с "KEY=" ?
				&& data->envp[i][key_len] == '=')
		{
			free(data->envp[i]); // Освобождаем память текущего элемента
			while (data->envp[i] != NULL) // Сдвигаем все элементы массива после i на одну позицию влево
			{
				data->envp[i] = data->envp[i + 1];
				i++;
			}
			if (data->nb_pipe == 0)//Если команда одиночная то просто выходим обратно в main
			{
				data->back_in_main = 1;
				data->exit_status = 0;
				return ;
			}
			else//Если команда составная то закрывем процесс и возвращаемся в родительский процесс
			{
				close (1);
				free_all_data(data);
			//	rl_clear_history();
				exit (0) ;
			}	
		}
		i++;
	}
	if (data->nb_pipe == 0)//Если команда одиночная то просто выходим обратно в main
	{
		data->back_in_main = 1;
		data->exit_status = -1;
		return ;
	}
	else//Если команда составная то закрывем процесс и возвращаемся в родительский процесс
	{
		close (1);
		free_all_data(data);
	//	rl_clear_history();
		exit (-1) ;
	}	
}