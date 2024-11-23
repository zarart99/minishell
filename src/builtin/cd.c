#include "../../include/minishell.h"

static void cd_exit(t_data *data, int status, char *message, char *to_free)
{
    if (message)
        write(2, message, ft_strlen(message));
    if (to_free)
        free(to_free); // Освобождаем переданный указатель, если он не NULL
    if (data->nb_pipe == 0)
    {
        data->back_in_main = 1;
        data->exit_status = status;
    }
    else
    {
        free_all_data(data);
        rl_clear_history();
        exit(status);
    }
}

// Получает текущую директорию
static char *get_current_directory(t_data *data)
{
    char *dir = getcwd(NULL, 0);

    if (!dir)
        cd_exit(data, 1, "cd: error retrieving current directory\n", NULL);
    return dir;
}

// Смена директории
static int change_directory(t_data *data, char *target_dir)
{
	if (chdir(target_dir) == -1)
	{
		perror("cd");
		cd_exit(data, 1, NULL, NULL);
		return -1;
	}
	return 0;
}

// Обновляет переменные окружения OLDPWD и PWD
static void update_environment(t_data *data, char *old_pwd)
{
    char *new_pwd;
    char *export_oldpwd = NULL;
    char *export_newpwd = NULL;

    new_pwd = get_current_directory(data);
    if (!new_pwd)
        return;
    export_oldpwd = ft_strjoin("OLDPWD=", old_pwd);
    if (export_oldpwd)
    {
        export_var(data, export_oldpwd);
        free(export_oldpwd); 
    }
    export_newpwd = ft_strjoin("PWD=", new_pwd);
    if (export_newpwd)
    {
        export_var(data, export_newpwd);
        free(export_newpwd); 
    }
    free(new_pwd);
}

void cd(t_data *data, char *arg)
{
    char *old_pwd;
    char *target_dir = NULL;
    int free_target_dir = 0; // Флаг, указывающий, нужно ли освобождать target_dir

    if (data->nb_pipe != 0)//Закрываем input для составных команд
		close_input(data);
    old_pwd = get_current_directory(data);
    if (!old_pwd)
        return;
    if (!arg) // Если аргумент отсутствует, используем "HOME"
    {
        target_dir = get_env_value("HOME", data);
        if (!target_dir)
            cd_exit(data, 1, "cd: HOME not set\n", old_pwd);
    }
    else if (ft_strcmp(arg, "-") == 0)
    {
        target_dir = get_env_value("OLDPWD", data);
        if (!target_dir)
            cd_exit(data, 1, "cd: OLDPWD not set\n", old_pwd);
        else
            ft_printf("%s\n", target_dir); // Печатаем OLDPWD
    }
    else if (arg[0] == '~')
    {
        char *home = get_env_value("HOME", data);
        if (!home)
            cd_exit(data, 1, "cd: HOME not set\n", old_pwd);
        else
        {
            target_dir = ft_strjoin(home, arg + 1);
            free_target_dir = 1; // Устанавливаем флаг, так как строка была создана
        }
    }
    else
        target_dir = arg;
    if (data->back_in_main == 1)
        return ;
    //Если выше в условиях неполучилось создать строку с адресом то cd_exit проставит 
    //data->back_in_main == 1 , значит когда выпадем с уловия то в случае ошибки здесь 
    //будет выход из функции.Я не прописывал выход в каждом условии что бы не забивать место 
    if (change_directory(data, target_dir) == 0)
        update_environment(data, old_pwd);
    if (data->back_in_main == 1)//Если change_directory или update_environment отработали не корректно то выходим из функции обратно в main
        return ;
    if (free_target_dir)    // Освобождаем target_dir только если оно было выделено динамически
        free(target_dir);
    cd_exit(data, 0, NULL, old_pwd);
}



