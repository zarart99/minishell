/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artemii <artemii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 00:47:25 by artemii           #+#    #+#             */
/*   Updated: 2024/10/10 00:47:25 by artemii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void ft_redirection_in(char *input_file)
{
    int fd_in = open(input_file, O_RDONLY);
    if (fd_in == -1)
    {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }
    if (dup2(fd_in, STDIN_FILENO) == -1)
    {
        perror("Error duplicating file descriptor for input");
        close(fd_in);
        exit(EXIT_FAILURE);
    }
    close(fd_in); // Закрываем дескриптор файла после перенаправления
}

void ft_redirection_out_append(char *output_file)
{
    int fd_out = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd_out == -1)
    {
        perror("Error opening output file in append mode");
        exit(EXIT_FAILURE);
    }
    if (dup2(fd_out, STDOUT_FILENO) == -1)
    {
        perror("Error duplicating file descriptor for append output");
        close(fd_out);
        exit(EXIT_FAILURE);
    }
    close(fd_out); // Закрываем дескриптор файла после перенаправления
}

void ft_redirection_out(char *output_file)
{
    int fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_out == -1)
    {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }
    if (dup2(fd_out, STDOUT_FILENO) == -1)
    {
        perror("Error duplicating file descriptor for output");
        close(fd_out);
        exit(EXIT_FAILURE);
    }
    close(fd_out); // Закрываем дескриптор файла после перенаправления
}