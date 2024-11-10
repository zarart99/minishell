/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_envp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artemii <artemii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 00:12:33 by artemii           #+#    #+#             */
/*   Updated: 2024/11/09 20:31:05 by artemii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//Тупо функция envp

//void print_env(void)
//{
//    for (int i = 0; global_envp[i] != NULL; i++) {
//        write(STDOUT_FILENO, global_envp[i], strlen(global_envp[i]));
//        write(STDOUT_FILENO, "\n", 1);
//    }
//}
void	print_env(t_data *data)
{
	int	i;

	i = 0;
	while (data->envp[i] != NULL)
	{
		ft_putstr_fd(data->envp[i], STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		i++;
	}
}