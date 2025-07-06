/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 22:56:11 by mzutter           #+#    #+#             */
/*   Updated: 2025/07/06 20:49:04 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_execsize(t_exec *exec)
{
	int		i;

	i = 0;
	while (exec)
	{
		exec = exec->next;
		i++;
	}
	return (i);
}

void	close_parent_fds(t_exec *tmp, int *pipe_fd, int *prev_fd_in)
{
	if (tmp->fd_in != STDIN_FILENO && tmp->fd_in != *prev_fd_in)
	{
		close(tmp->fd_in);
		tmp->fd_in = STDIN_FILENO;
	}
	if (tmp->fd_out != STDOUT_FILENO)
	{
		close(tmp->fd_out);
		tmp->fd_out = STDOUT_FILENO;
	}
	if (*prev_fd_in != STDIN_FILENO && *prev_fd_in != tmp->fd_in)
	{
		close(*prev_fd_in);
		*prev_fd_in = STDIN_FILENO;
	}
	if (tmp->next != NULL)
	{
		close(pipe_fd[1]);
		if (tmp->next->fd_in == STDIN_FILENO)
			tmp->next->fd_in = pipe_fd[0];
		else
			close(pipe_fd[0]);
	}
	*prev_fd_in = tmp->fd_in;
}

void	wait_for_children_to_exit(t_shell *shell, pid_t last_pid)
{
	int		status;
	pid_t	wpid;
	int		sig;

	while ((wpid = wait(&status)) > 0)
	{
		if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			shell->exit_status = 128 + sig;
			if (sig == SIGQUIT)
				write(1, "Quit\n", 5);
			if (sig == SIGINT)
				write(1, "\n", 1);
		}
		else if (wpid == last_pid && WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
	}
}

void	wait_for_heredoc_to_exit(pid_t pid)
{
	int		status;
	int		sig;
	
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
		{
			write(1, "\n", 1);
			g_signal = SIGINT;
		}
	}
}

void	exec_loop(t_shell *shell)
{
	t_exec	*tmp;
	pid_t	pid;
	int		pipe_fd[2];
	int		prev_fd_in;
	pid_t	last_pid;

	prev_fd_in = STDIN_FILENO;
	tmp = shell->exec;
	if (tmp->arr && ft_execsize(tmp) == 1)
		update_or_add("_", tmp->arr[count_strings(tmp->arr) - 1], shell, 0);
	if (tmp->arr && ft_execsize(tmp) == 1 && ft_is_builtin(tmp->arr[0]))
	{
		shell->exit_status = handle_builtin(shell, tmp);
		return ;
	}
	while (tmp)
	{
		if (!is_valid_command(tmp))
		{
			tmp = tmp->next;
			continue ;
		}
		if (tmp->next != NULL)
			safe_pipe(pipe_fd, shell);
		signal(SIGINT, SIG_IGN);
		pid = safe_fork(shell);
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
    		signal(SIGQUIT, SIG_DFL);
			handle_child_process(shell, tmp, pipe_fd);
		}
		else
		{
			last_pid = pid;
			close_parent_fds(tmp, pipe_fd, &prev_fd_in);
		}			
		tmp = tmp->next;
	}
	if (prev_fd_in != STDIN_FILENO)
		close(prev_fd_in);
	wait_for_children_to_exit(shell, last_pid);
	signal(SIGINT, sigint_handler);
}
