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
# include <signal.h>
# include <limits.h>

typedef struct s_cmd
{
    char *input_file;     // Для "<"
    int pos_input;        // Позиция для приоритета входного редиректа
    char *here_doc_file;  // Для "<<"
    int pos_here_doc;     // Позиция для приоритета here_doc
    char *cmd;            // Основная команда (например, "echo")
    char **cmd_arg;        // Аргументы команды (например, "hello world")
    char *output_file;    // Для ">"
    char *append_file;    // Для ">>"
    int pos_output;       // Позиция для приоритета вывода
    int pos_append;       // Позиция для приоритета append
    int here_doc_pfd; // канал для чтения данных принятых here_doc ,    
    char **output_files;    // Массив для ">"
    char **append_files;    // Массив для ">>" 
    char **here_doc_files;  // Массив для "<<"
    char **input_files;     // Массив для "<"
} t_cmd;

typedef struct s_data
{
    char **envp;      // Переменные окружения
    t_cmd **cmd;      // Массив структур команд
    int nb_pipe;      // Количество пайпов
    int prev_pipe;    // Флаг пайпа для предыдущей команды
    int i;            //Индекс текущей команды                                   //add
    int flag_pipe;    //Что бы определить если заполненый пайп                                      //add
    int exit_status;  //Сохраняем индекс последнего процесса запущеной команды

    int heredoc_interrupted; //Для входа из here doc 
    int back_in_main;        //Для возврата в main после исполнения наших одиночных команд
    char *user_input;     // Что вводит пользователь
    int builtin_cmd;
    int display_builtin_cmd;
    int	pipefd[2];
    int std_out;
} t_data;

extern int g_pid;

void		parse_pipeline(t_data *command, char *input);
void		free_parsed_commands(t_data **commands);
char		*find_command(char *cmd, char **envp);
void		free_split(char **args);
void		error_exit(const char *message);
char        **ft_split_quotes(const char *input, t_data *data);
void		free_structure(t_data *command);

// Функции для работы с переменными окружения
char	    *replace_env_var(char *input, t_data *data);
char	    *replace_substring(char *str, int start, int end, char *replacement);
void        unset_var(t_data *data, char *key);
void        export_var(t_data *data, const char *input);
void	    print_env(t_data *data);
void        cd(t_data *data, char *args);
char	    *get_env_value(char *name, t_data *data);




int         ft_strncmp(const char *s1, const char *s2, size_t n);
char        *get_next_line(int fd);
char        *ft_strjoin(char const *s1, char const *s2);
char        **ft_split(char const *s, char c);
int         ft_strlen(const char *str);
size_t      ft_strlcpy(char *dest, const char *src, size_t size);
char        *ft_strnstr(const char *big, const char *little, size_t len);
char        *ft_strdup(const char *s);
char        *ft_envp_cherch(char *cmd, char **envp);
char        *ft_creat_path(char **strs, char *cmd);
void        ft_add_cmd(char **strs, char *cmd);
void        ft_add_symb(char **strs);

void		choice_execution(t_data *data);                             //Здесь будет происходить выбор между нашими командами и системными
void		execution_cmd(t_data *data);
void	    manage_fd(t_data *data, int pid);                                //Функция где создаются дочерние процессы и пайпы
void	    ft_launch_cmd(t_data *data);                 //Запускает исполнение системной команды + редирекции
void	    check_file_args(t_data *data);
void	    wait_processes(t_data *data);
                               
void	    execution_here_doc(t_cmd *cmd, char *here_doc_file, t_data *data);                          //Если есть here_doc то это функция которая его исполняет
void	    read_line_here_doc(char *here_doc_file, int pipefd);

void        redirection(t_data *data);            //Функция определяет какой из 4 вариантов output нужно задействовать для текущей команды

void        ft_redirection_in(t_data *data);             //Переадресация из файла в команду
void        ft_redirection_here_doc(t_data *data);       //Переадресация из here_doc в команду
void	    manage_fd_redirection_input(t_data *data, int flag_heredoc);
int	        check_input_files(t_data *data, int flag_heredoc);
void        redirection_input_stdin(t_data *data, int fd_in);
void        redirection_here_doc_stdin(t_data *data);


void        ft_redirection_pipe(t_data *data);           //Переадресация из пайпа в команду
void        ft_redirection_out_cmd(t_data *data);        //Переадресация из команды в файл / 2 варианта файла
void	    manage_fd_redirection_output(t_data *data);
int         check_output_files(t_data *data);
int	        check_append_files(t_data *data);
int         redirection_output_stdout(t_data *data, int fd_out);
int         check_files(t_data *data, int flag);
int         redirection_output(t_data *data, int fd_out);

void        ft_redirection_out_pipe(t_data *data);       //Переадресация из команды в пайп

/*Блок для корректного выхода из процесса из за ошибок */
void	free_fault_execve(char *cmd, t_data *data);
void		ft_free_strs(char **strs);
void		ft_error_exit(int nb);
void		free_pipe(int fd);
void	    error_join_arg(t_data *data);
void		error_empty_cmd(t_data *data);
void		free_error_cmd(t_data *data);
void	    error_open_outfile(int flag , t_data *data);


void	free_data(t_data *data); //Этой функции больше нет? 


//Сигналы
void    handle_sigint(int sig);

//Наши функции 
void	execute_builtin_command(t_data *data);
void	check_builtin_command(t_data *data);
void	redirection_builtin_command(t_data *data);
void	launch_func_builtin(t_data *data);
void	remove_stdout(t_data *data);
//echo
void	echo(t_data *data);
int	    check_option_n(char *arg);
void	display_str(t_data *data, int i_2, int n);

//exit
void	exit_total(t_data *data);
void	check_isdigit(t_data *data);
int     ft_atoi_mod(const char *nptr);

void	pwd(t_data *data);

void	close_input(t_data *data);//Закрываем переадресацию на вход для наших функций 

//void	error_open_outfile(int flag);//Проверить нужен ли ?

void    free_all_data(t_data *data);
void	free_data_cmd(t_data *data);

void sigint_heredoc(t_data *data, int pipefd[2], int in);

int	    exit_cmds_builtin(t_data *data, char *error, int status, char *most_by_free);
void	close_here_doc_pfd(int fd);
void	exit_eof(char *lim);
void	exit_only_child(t_data *data, int status);
void	error_fork(t_data *data);
int     exit_open(t_data *data, int fd_out);
#endif
