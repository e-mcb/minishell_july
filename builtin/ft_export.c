/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 22:32:04 by sradosav          #+#    #+#             */
/*   Updated: 2025/07/06 19:00:26 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_write_export(char *str, int fd_out)
{
	int	i;

	write(fd_out, "declare -x ", 11);
	i = 0;
	while (str[i] != 0 && str[i] != '=')
	{
		write(fd_out, &str[i], 1);
		i++;
	}
	if (str[i] == '=')
	{
		write(fd_out, &str[i], 1);
		write(fd_out, "\"", 1);
		i++;
		while (str[i] != 0)
		{
			if (str[i] == '"')
				write(fd_out, "\"", 1);
			write(fd_out, &str[i], 1);
			i++;
		}
		write(fd_out, "\"", 1);
	}
	write(fd_out, "\n", 1);
}

void	ft_print_export(t_shell *shell, int fd_out)
{
	t_envvar	*env_copy;
	t_envvar	*iter;

	env_copy = copy_env_list(shell->env, shell);
	ft_sort_env_list(env_copy);
	iter = env_copy;
	while (iter)
	{
		if (iter->exported == 1)
			ft_write_export(iter->var, fd_out);
		iter = iter->next;
	}
	free_env_list(&env_copy);
}

void	handle_valid_export(char *str, t_shell *shell)
{
	int		var_len;
	char	*var;
	char	*value;

	var_len = 0;
	while (str[var_len] && str[var_len] != '=')
		var_len++;
	var = ft_strndup(str, var_len);
	if (!var)
		ft_clean_exit(NULL, shell, NULL, NULL);
	if (ft_strchr(str, '='))
		value = ft_strchr(str, '=') + 1;
	else
		value = "";
	update_or_add(var, value, shell, 1);
	free(var);
}

int	ft_export_vars(char **str, t_shell *shell)
{
	int		i;

	i = 1;
	while (str[i])
	{
		if (!is_valid_identifier(str[i]))
		{
			printf("minishell: export: `%s': not a valid identifier\n", str[i]);
			return (1);
		}
		else
		{
			handle_valid_export(str[i], shell);
			return (0);
		}
		i++;
	}
	return (0);
}

int	ft_export(char **str, t_shell *shell, int in_pipeline, int fd_out)
{
	if (!str[1])
	{
		ft_print_export(shell, fd_out);
		return (0);
	}
	else if (in_pipeline == 1)
		return (ft_export_vars(str, shell));
	return (0);
}
