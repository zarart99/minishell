/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artemii <artemii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 16:16:35 by azakharo          #+#    #+#             */
/*   Updated: 2024/10/18 00:38:20 by artemii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

extern char	**environ;

void print_commands(t_command *command)
{
    int i, j;

    i = 0;
    while (command->cmd[i] != NULL)
    {
        ft_printf("Command[%d]:\n", i);
        j = 0;
        while (command->cmd[i][j] != NULL)
        {
            ft_printf("  Args[%d]: %s\n", j, command->cmd[i][j]);
            j++;
        }
        i++;
    }
    if (command->input_file)
        ft_printf("  Input file: %s\n", command->input_file);
    if (command->output_file)
        ft_printf("  Output file: %s\n", command->output_file);
    if (command->append)
        ft_printf("  Append mode: enabled\n");
    if (command->here_doc)
        ft_printf("  Here_doc: enabled\n");
    if (command->is_pipe)
        ft_printf("  Is pipe: enabled\n");
}


void free_structure(t_command *cmd) //Функция для чистки структуры
{
    int i = 0;
    
    // Освобождаем каждый массив аргументов
    if (cmd->cmd) 
    {
        while (cmd->cmd[i]) 
        {
            free_split(cmd->cmd[i]);  // Освобождаем каждый массив
            i++;
        }
        free(cmd->cmd);  // Освобождаем сам массив команд
    }
    // Освобождаем файлы редиректов
    if (cmd->input_file)
        free(cmd->input_file);
    if (cmd->output_file)
        free(cmd->output_file);
    // Освобождаем саму структуру
    free(cmd);
}

int main(void)
{
    char *user_input;
    t_command *command;

    while (1)
    {
        user_input = readline("minishell$ ");
        if (!user_input)
            break;
        if (ft_strlen(user_input) > 0)
            add_history(user_input);
        command = malloc(sizeof(t_command));
        if (!command)
        {
            perror("malloc failed");
            return (1);
        }
        ft_memset(command, 0, sizeof(t_command));
        parse_pipeline(command, user_input);

        print_commands(command); // Вывод в консоль для дебага
        //ВОТ СЮДА НАДО ДОБАВИТЬ EXECUTION
        free_structure(command);
        free(user_input);
    }

    rl_clear_history();
    return 0;
}

