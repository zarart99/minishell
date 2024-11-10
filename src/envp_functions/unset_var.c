/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_var.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artemii <artemii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 00:06:56 by artemii           #+#    #+#             */
/*   Updated: 2024/11/09 20:30:42 by artemii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


//// Удаление переменной из global_envp
//int unset_var(const char *key)
//{
//    int key_len = strlen(key);

//    for (int i = 0; global_envp[i] != NULL; i++) {
//        if (strncmp(global_envp[i], key, key_len) == 0 && global_envp[i][key_len] == '=') {
//            free(global_envp[i]);
//            while (global_envp[i + 1] != NULL) {
//                global_envp[i] = global_envp[i + 1];
//                i++;
//            }
//            global_envp[i] = NULL;
//            return 0;
//        }
//    }
//    return -1;
//}


// Удаление переменной из окружения
int	unset_var(t_data *data, char *key)
{
	int		key_len;
	int		i;

	key_len = ft_strlen(key);
	i = 0;
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
			return (0);
		}
		i++;
	}
	return (-1); // Если не нашли переменную
}