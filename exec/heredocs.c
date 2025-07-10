/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sradosav <sradosav@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 22:45:52 by mzutter           #+#    #+#             */
/*   Updated: 2025/07/03 19:48:41 by sradosav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*do_heredoc(const t_token *token, t_shell *shell)
{
	int		pipefd[2];
	pid_t	pid;
	char	*finale;
	char	*line;
	char	*tmp;

	finale = NULL;
	safe_pipe(pipefd, shell);
	signal(SIGINT, SIG_IGN);
	pid = safe_fork(shell);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
		close(pipefd[0]);
		while (1)
		{
			char	*input = readline("> ");
			if (!input)
			{
				ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted '", 2);
				ft_putstr_fd(token->value, 2);
				ft_putstr_fd("')\n", 2);
				break ;
			}
			if (strcmp(input, token->value) == 0)
			{
				free(input);
				break ;
			}
			write(pipefd[1], input, strlen(input));
			write(pipefd[1], "\n", 1);
			free(input);
		}
		close(pipefd[1]);
		exit(0);
	}
	else
	{
		close(pipefd[1]);
		while ((line = get_next_line(pipefd[0])) != NULL)
		{
			if (finale)
			{
				tmp = finale;
				finale = ft_strjoin(finale, line);
				free(tmp);
			}
			else
				finale = ft_strdup(line);
			if (!finale)
				return (NULL);
			free(line);
		}
		close(pipefd[0]);
		wait_for_heredoc_to_exit(pid);
		signal(SIGINT, sigint_handler);
		return (finale);
	}
}

// char	*do_heredoc(const t_token *token, const t_shell *shell)
// {
// 	int		pipefd[2];
// 	pid_t	pid;
// 	char	*finale;
// 	char	*line;
// 	char	*tmp;

// 	finale = NULL;
// 	pipe(pipefd);
// 	pid = fork();
// 	g_status = 1;
// 	if (pid == 0)
// 	{
// 		signal(SIGINT, SIG_DFL);
// 		signal(SIGQUIT, SIG_IGN);
// 		close(pipefd[0]);
// 		while (1)
// 		{
// 			char *input = readline("> ");
// 			if (!input)
// 			{
// 				ft_putstr_fd("minishell: warning: here-document delimited by end-of-file.\n", 2);
// 				break ;
// 			}
// 			if (strcmp(input, token->value) == 0)
// 			{
// 				free(input);
// 				break ;
// 			}
// 			write(pipefd[1], input, strlen(input));
// 			write(pipefd[1], "\n", 1);
// 			free(input);
// 		}
// 		close(pipefd[1]);
// 		exit(0);
// 	}
// 	else
// 	{
// 		close(pipefd[1]);
// 		while ((line = get_next_line(pipefd[0])) != NULL)
// 		{
// 			if (finale)
// 			{
// 				tmp = finale;
// 				finale = ft_strjoin(finale, line);
// 				free(tmp);
// 			}
// 			else
// 				finale = ft_strdup(line);
// 			if (!finale)
// 				return NULL;
// 			free(line);
// 		}
// 		close(pipefd[0]);
// 		// wait_for_children_to_exit(shell, 1);
// 		return (finale);
// 	}
// }
