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

typedef struct s_command
{
	char **envp; // Переменные окружения
	char	***cmd;
	// Массив массивов команд (каждая команда — это массив аргументов)
	char *input_file;  // Файл для входного редиректа (<)
	char *output_file; // Файл для выходного редиректа (>)
	int append;        // Флаг для редиректа с добавлением (>>)
	int here_doc;      // Флаг для here_doc (<<)
	int is_pipe;       // Флаг пайпа
	int prev_pipe;     // Для отслеживания пайпов
}			t_command;

void		parse_pipeline(t_command *command, char *input);
// void	execute_pipeline(char ***commands, char **envp);
void		free_parsed_commands(t_command **commands);
char		*find_command(char *cmd, char **envp);
void		free_split(char **args);
void		error_exit(const char *message);
void		launch_here_doc(char **argv, int pipefd[2]);
void		ft_redirection_in(char *input_file);
void		ft_redirection_out_append(char *output_file);
void		ft_redirection_out(char *output_file);
void		execute_pipeline(t_command *commands);
char		**ft_split_quotes(const char *input);
void		free_structure(t_command *command);

#endif
