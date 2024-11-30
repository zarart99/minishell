/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artemii <artemii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 00:47:05 by artemii           #+#    #+#             */
/*   Updated: 2024/11/30 22:52:50 by artemii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void process_tokens(t_cmd *cmd, char **tokens)
{
	int	i;
	int	arg_idx;
	int	redir_position;

	i = 0;
	arg_idx = 0;
	redir_position = 1;

	while (tokens[i] != NULL)
	{
		if (has_redirection(tokens[i]))
		{
			process_redirection_token(cmd, tokens[i], &redir_position, &i, tokens, &arg_idx);
		}
		else
		{
			handle_command_args(cmd, tokens, &i, &arg_idx);
		}
		i++;
		if (cmd->error_code == 2)
			return;
	}

	// Завершаем массив аргументов
	if (arg_idx > 0)
		cmd->cmd_arg[arg_idx] = NULL;
	else
		cmd->cmd_arg = NULL;
}





void	parse_single_command(t_cmd *cmd, char *input, t_data *data)
{
	char	**tokens;

	tokens = ft_split_quotes(input, data);
	if (!tokens)
		return ;
	process_tokens(cmd, tokens);
	free_split(tokens);
	if (cmd-> error_code)
	{
		data->back_in_main = 1;
		data->exit_status = cmd->error_code;
		return ;
	}
	handle_here_docs(cmd, data);
}

static int	initialize_pipeline(t_data *data, char *input,
		char ***command_tokens)
{
	int	cmd_count;

	if (!validate_quotes(input))
	{
		data->back_in_main = 1;
		ft_printf("Error: unclosed quotes\n");
		return (-1);
	}
	*command_tokens = ft_split(input, '|');
	if (!(*command_tokens))
		return (-1);
	cmd_count = 0;
	while ((*command_tokens)[cmd_count] != NULL)
		cmd_count++;
	data->cmd = malloc(sizeof(t_cmd *) * (cmd_count + 1));
	if (!data->cmd)
	{
		free_split(*command_tokens);
		return (-1);
	}
	for (int i = 0; i <= cmd_count; i++) {
    data->cmd[i] = NULL;
}
	data->cmd[cmd_count] = NULL;
	data->nb_pipe = cmd_count - 1;
	return (cmd_count);
}

static void	process_pipeline_commands(t_data *data, char **command_tokens,
		int cmd_count)
{
	int	i;

	i = 0;
	while (i < cmd_count)
	{
		data->cmd[i] = malloc(sizeof(t_cmd));
		if (!data->cmd[i])
		{
			perror("malloc failed");
			free_split(command_tokens);
			free_data_cmd(data);
			return ;
		}
		ft_memset(data->cmd[i], 0, sizeof(t_cmd));
		parse_single_command(data->cmd[i], command_tokens[i], data);
		if (data->back_in_main == 1)
		{
			return ;
		}
		i++;
	}
}

void	parse_pipeline(t_data *data, char *input)
{
	char	**command_tokens;
	int		cmd_count;

	cmd_count = initialize_pipeline(data, input, &command_tokens);
	if (cmd_count == -1)
		return ;
	process_pipeline_commands(data, command_tokens, cmd_count);
	free_split(command_tokens);
	if (data->back_in_main == 1)
		return;
 
}