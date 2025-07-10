#include "../includes/minishell.h"

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

int	should_run_single_builtin(t_shell *shell, t_exec *cmd)
{
	int	arr_size;

	if (!cmd)
	{
		return (0);
	}
	if (!cmd->arr)
	{
		return (0);
	}
	arr_size = ft_execsize(cmd);
	if (arr_size != 1)
	{
		return (0);
	}
	update_or_add("_", cmd->arr[count_strings(cmd->arr) - 1], shell, 0);
	if (!ft_is_builtin(cmd->arr[0]))
	{
		return (0);
	}
	shell->exit_status = handle_builtin(shell, cmd);
	return (1);
}

void	handle_in_child(t_shell *shell, t_exec *cmd, int *pipe_fd)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	handle_child_process(shell, cmd, pipe_fd);
}

pid_t	execute_all_commands(t_shell *shell, t_exec *tmp,
		int *pipe_fd, int prev_fd_in)
{
	pid_t	pid;
	pid_t	last_pid;

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
			handle_in_child(shell, tmp, pipe_fd);
		else
		{
			last_pid = pid;
			close_parent_fds(tmp, pipe_fd, &prev_fd_in);
		}
		tmp = tmp->next;
	}
	return (last_pid);
}
