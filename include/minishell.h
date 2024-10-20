#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_cmd
{
    char *input_file;     // Для "<"
    int pos_input;        // Позиция для приоритета входного редиректа
    char *here_doc_file;  // Для "<<"
    int pos_here_doc;     // Позиция для приоритета here_doc
    char *cmd;            // Основная команда (например, "echo")
    char *cmd_arg;        // Аргументы команды (например, "hello world")
    char *output_file;    // Для ">"
    char *append_file;    // Для ">>"
    int pos_output;       // Позиция для приоритета вывода
    int pos_append;       // Позиция для приоритета append
} t_cmd;

typedef struct s_data
{
    char **envp;      // Переменные окружения
    t_cmd **cmd;      // Массив структур команд
    int nb_pipe;      // Количество пайпов
    int prev_pipe;    // Флаг пайпа для предыдущей команды
} t_data;


void		parse_pipeline(t_data *command, char *input);
// void	execute_pipeline(char ***commands, char **envp);
void		free_parsed_commands(t_data **commands);
char		*find_command(char *cmd, char **envp);
void		free_split(char **args);
void		error_exit(const char *message);
void		launch_here_doc(char **argv, int pipefd[2]);
void		ft_redirection_in(char *input_file);
void		ft_redirection_out_append(char *output_file);
void		ft_redirection_out(char *output_file);
void		execute_pipeline(t_data *commands);
char		**ft_split_quotes(const char *input);
void		free_structure(t_data *command);

#endif
