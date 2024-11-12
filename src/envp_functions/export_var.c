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

int	export_var(t_data *data, const char *input)
{
	char	*key;
	char	*new_var;

	if (!input)
		print_env(data); // Если ввели просто export без аргумента - должны вывести что есть в envp
	key = get_key(input); // Получаем ключ
	if (!key)
		return (-1); // Некорректный формат
	new_var = ft_strdup(input); // Создаем "key=value"
	if (!new_var)
	{
		free(key);
		return (-1);
	}
	if (update_or_add_var(data, new_var, key) == -1) // Обновляем или добавляем переменную
	{
		free(new_var);
		free(key);
		return (-1);
	}
	free(key);
	return (0);
}
