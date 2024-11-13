#include "../../include/minishell.h"

static char	*get_key(const char *input) // Извлечение ключа из "key=value"
{
	char	*equal_pos;
	equal_pos = ft_strchr(input, '='); // Ищем '='
	if (!equal_pos)
		return (NULL);
	return (ft_substr(input, 0, equal_pos - input)); // Возвращаем ключ
}

static int	update_or_add_var(t_data *data, char *new_var, char *key)
{
	int		i;
	char	**new_envp;

	i = 0;
	while (data->envp[i]) // Проход по envp
	{
		if (ft_strncmp(data->envp[i], key, ft_strlen(key)) == 0 &&
			data->envp[i][ft_strlen(key)] == '=') // Совпадение ключа
		{
			free(data->envp[i]);
			data->envp[i] = new_var; // Обновление
			return (0);
		}
		i++;
	}
	new_envp = ft_realloc(data->envp, sizeof(char *) * (i + 1),
			sizeof(char *) * (i + 2)); // Расширение envp
	if (!new_envp)
		return (-1);
	new_envp[i] = new_var; // Добавление новой переменной
	new_envp[i + 1] = NULL;
	data->envp = new_envp;
	return (0);
}

void	export_var(t_data *data, const char *input)
{
	char	*key;
	char	*new_var;

	if (data->nb_pipe != 0)//Закрываем input для составных команд
		close_input(data);
	if (!input)
		print_env(data); // Если ввели просто export без аргумента - должны вывести что есть в envp
	else
	{
		key = get_key(input); // Получаем ключ
		if (!key)
		{
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
	//			rl_clear_history();
				exit (-1) ;
			}	
		}
		new_var = ft_strdup(input); // Создаем "key=value"
		if (!new_var)
		{
			if (data->nb_pipe == 0)
			{
				free(key);
				data->back_in_main = 1;
				data->exit_status = -1;
				return ;
			}
			else
			{
				close (1);
				free(key);
				free_all_data(data);
		//		rl_clear_history();
				exit (-1) ;
			}	
		}
		if (update_or_add_var(data, new_var, key) == -1) // Обновляем или добавляем переменную
		{
			if (data->nb_pipe == 0)
			{
				free(new_var);
				free(key);
				data->back_in_main = 1;
				data->exit_status = -1;
				return ;
			}
			else
			{
				close (1);
				free(new_var);
				free(key);
				free_all_data(data);
		//		rl_clear_history();
				exit (-1) ;
			}	
		}
		if (data->nb_pipe == 0)
		{
			free(key);
			data->back_in_main = 1;
			data->exit_status = 0;
			return ;
		}	
		else
		{
			close (1);
			free(key);
			free_all_data(data);
//			rl_clear_history();
			exit (0) ;
		}	
	}
}
