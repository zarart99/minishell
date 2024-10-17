#include "../include/minishell.h"

void	free_split(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

// Обработка редиректов и пайпов
void handle_redirections_and_pipes(t_command *command, char **tokens, int *i, int *arg_in_cmd, char **args)
{
    if (ft_strcmp(tokens[*i], "<") == 0)  // Входной редирект
    {
        *i += 1;
        command->input_file = ft_strdup(tokens[*i]);
    }
    else if (ft_strcmp(tokens[*i], ">") == 0)  // Выходной редирект
    {
        *i += 1;
        command->output_file = ft_strdup(tokens[*i]);
    }
    else if (ft_strcmp(tokens[*i], ">>") == 0)  // Выходной редирект с добавлением в файл
    {
        *i += 1;
        command->output_file = ft_strdup(tokens[*i]);
        command->append = 1;
    }
    else if (ft_strcmp(tokens[*i], "<<") == 0)  // Here-doc
    {
        args[(*arg_in_cmd)++] = ft_strdup("<<"); // Кладём в первый аргумент команды
        *i += 1;
        args[(*arg_in_cmd)++]= ft_strdup(tokens[*i]);  // Лимитер кладём во второй аргумент
    }
}


void parse_input(t_command *command, char *input, int cmd_idx)
{
    int i = 0;
    int arg_in_cmd = 0;
    char *full_command = NULL;  // Переменная для объединения аргументов
    char **args = malloc(sizeof(char *) * 100);  // Допустим максимум 100 аргументов

    if (!args)
        return;
    char **tokens = ft_split_quotes(input);

    while (tokens[i] != NULL)
    {
        if (ft_strcmp(tokens[i], "<") == 0 || ft_strcmp(tokens[i], ">") == 0 || 
            ft_strcmp(tokens[i], ">>") == 0 || ft_strcmp(tokens[i], "<<") == 0)
            handle_redirections_and_pipes(command, tokens, &i, &arg_in_cmd, args);  
        else 
        {
            if (full_command == NULL)
                full_command = ft_strdup(tokens[i]);  // Если это первый аргумент, просто копируем его
            else
            {
                full_command = ft_strjoin(full_command, " ");  // Добавляем пробел
                full_command = ft_strjoin(full_command, tokens[i]);  // Добавляем следующий аргумент
            }
        }
        i++;
    }
    if (full_command != NULL)
        args[arg_in_cmd++] = full_command; //Кладём команду в элемент массива команды
    args[arg_in_cmd] = NULL;
    command->cmd[cmd_idx] = args;
    free_split(tokens);  // Освобождаем временный массив токенов
}

void parse_pipeline(t_command *command, char *input)
{
    char **command_tokens;
    int cmd_count = 0;
    int i = 0;

    // Разбиваем строку на команды по пайпам
    command_tokens = ft_split(input, '|');  // Команды между пайпами
    while (command_tokens[cmd_count] != NULL)
        cmd_count++;
    command->cmd = malloc(sizeof(char **) * (cmd_count + 1));

    // Для каждой команды вызываем parse_input и кладём в массив команд
    while (command_tokens[i] != NULL)
    {
        parse_input(command, command_tokens[i], i);  // Передаем индекс команды `i`
        i++;
    }
    command->cmd[i] = NULL;  // Завершаем массив массивов
    free_split(command_tokens);  // Освобождаем временный массив токенов
}
