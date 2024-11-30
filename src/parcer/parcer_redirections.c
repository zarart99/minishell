/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcer_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artemii <artemii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 00:47:10 by artemii           #+#    #+#             */
/*   Updated: 2024/11/30 22:52:31 by artemii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**realloc_array(char **array, char *new_element)
{
	int		size;
	int		i;
	char	**new_array;

	size = 0;
	if (array)
		while (array[size])
			size++;
	new_array = malloc(sizeof(char *) * (size + 2));
	if (!new_array)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (i < size)
	{
		new_array[i] = array[i];
		i++;
	}
	new_array[size] = new_element;
	new_array[size + 1] = NULL;
	free(array);
	return (new_array);
}

void	update_redirection(char **target_file, char ***file_array, char *token)
{
	if (*target_file)
		free(*target_file);
	*target_file = ft_strdup(token);
	*file_array = realloc_array(*file_array, ft_strdup(token));
}

int	has_redirection(const char *token)
{
	return (ft_strchr(token, '<') != NULL || ft_strchr(token, '>') != NULL);
}

void	split_token_on_redirection(char *token, char **before, char **redir,
		char **after)
{
	int	i;
	int	redir_len;

	*before = NULL;
	*redir = NULL;
	*after = NULL;
	if (!token || !*token)
		return ;
	i = 0;
	while (token[i] && !ft_strchr("<>", token[i]))
		i++;
	if (token[i])
	{
		if (i > 0)
			*before = ft_strndup(token, i);
		if (token[i + 1] == '>' || token[i + 1] == '<')
			redir_len = 2;
		else
			redir_len = 1;
		*redir = ft_strndup(token + i, redir_len);
		*after = ft_strdup(token + i + redir_len);
	}
	else
		*before = ft_strdup(token);
}

void	process_redirection_token(t_cmd *cmd, char *token, int *redir_position,
		int *i, char **tokens, int *arg_idx)
{
	char	*before;
	char	*redir;
	char	*after;

	before = NULL;
	redir = NULL;
	after = NULL;
	if (!token)
		return ;
	split_token_on_redirection(token, &before, &redir, &after);
	if (before && *before != '\0')
	{
		if (!cmd->cmd_arg)
		{
			cmd->cmd_arg = malloc(sizeof(char *) * 100);
			if (!cmd->cmd_arg)
				return ;
			ft_bzero(cmd->cmd_arg, sizeof(char *) * 100);
		}
		cmd->cmd_arg[*arg_idx] = ft_strdup(before);
		(*arg_idx)++;
	}
	if (redir && (!after || *after == '\0'))
	{
		if (tokens[*i + 1])
		{
			(*i)++;
			free(after);
			after = ft_strdup(tokens[*i]);
		}
		else
		{
			ft_printf("Error: missing file for redirection '%s'\n", redir);
			cmd->error_code = 2;
			free(before);
			free(redir);
			free(after);
			return ;
		}
	}
	if (redir)
	{
		if (ft_strcmp(redir, "<") == 0)
		{
			cmd->pos_input = (*redir_position)++;
			update_redirection(&cmd->input_file, &cmd->input_files, after);
		}
		else if (ft_strcmp(redir, ">") == 0)
		{
			cmd->pos_output = (*redir_position)++;
			update_redirection(&cmd->output_file, &cmd->output_files, after);
		}
		else if (ft_strcmp(redir, ">>") == 0)
		{
			cmd->pos_append = (*redir_position)++;
			update_redirection(&cmd->append_file, &cmd->append_files, after);
		}
		else if (ft_strcmp(redir, "<<") == 0)
		{
			cmd->pos_here_doc = (*redir_position)++;
			update_redirection(&cmd->here_doc_file, &cmd->here_doc_files,
				after);
		}
	}
	free(before);
	free(redir);
	free(after);
}

void	handle_command_args(t_cmd *cmd, char **tokens, int *i, int *arg_idx)
{
	int	j;

	if (!cmd || !tokens || (i && !tokens[*i]))
		return ;
	if (cmd->cmd_arg == NULL)
	{
		cmd->cmd_arg = malloc(sizeof(char *) * 100);
		if (!cmd->cmd_arg)
			return ;
		for (j = 0; j < 100; j++)
			cmd->cmd_arg[j] = NULL;
		cmd->cmd = ft_strdup(tokens[*i]);
		if (!cmd->cmd)
		{
			free(cmd->cmd_arg);
			cmd->cmd_arg = NULL;
			return ;
		}
	}
	if (i && tokens[*i])
	{
		cmd->cmd_arg[*arg_idx] = ft_strdup(tokens[*i]);
		(*arg_idx)++;
	}
}

void	handle_here_docs(t_cmd *cmd, t_data *data)
{
	int	i;

	if (cmd->here_doc_file == NULL)
		return ;
	i = 0;
	while (cmd->here_doc_files[i] != NULL)
	{
		execution_here_doc(cmd, cmd->here_doc_files[i], data);
		if (data->back_in_main == 1)
			return ;
		i++;
	}
}
