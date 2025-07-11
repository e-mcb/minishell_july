/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 22:31:57 by sradosav          #+#    #+#             */
/*   Updated: 2025/06/23 22:30:26 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

long long	is_out_of_range(const char *nptr)
{
	long long			i;
	unsigned long long	r;
	long long			s;

	i = 0;
	r = 0;
	s = 1;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	if (nptr[i] == 45)
	{
		s = -1;
		i++;
	}
	else if (nptr[i] == 43)
		i++;
	while (nptr[i] >= 48 && nptr[i] <= 57)
	{
		r = (r * 10) + (nptr[i] - 48);
		i++;
	}
	if ((r > 9223372036854775808ULL && s == -1)
		|| (r > 9223372036854775807ULL && s == 1))
		return (1);
	return (0);
}

int	ft_exit_argcheck(int c)
{
	if ((c >= '0') && (c <= '9'))
		return (42);
	else
		return (0);
}

int	ft_is_number(const char *str)
{
	if (*str == '+' || *str == '-')
		str++;
	if (*str == '\0')
		return (0);
	while (*str)
	{
		if (!ft_exit_argcheck(*str))
			return (0);
		str++;
	}
	return (1);
}

void	free_before_exit(t_shell *shell, void *ptr_a, void *ptr_b)
{
	(void)ptr_a;
	(void)ptr_b;
	if (shell)
	{
		if (shell->env)
			free_env_list(&(shell->env));
		if (shell->env_arr)
			ft_free_str_array(shell->env_arr);
		if (shell->splitted)
			ft_free_str_array(shell->splitted);
		if (shell->token)
			free_list(&(shell->token));
		if (shell->exec)
			free_exec_list(&(shell->exec));
		free(shell);
	}
}

int	ft_exit(char **arr, t_shell *shell)
{
	long long	exit_status;

	if (!arr[1])
	{
		free_before_exit(shell, NULL, NULL);
		ft_putstr_fd("exit\n", 1);
		exit(0);
	}
	else if (arr[1] && arr[2])
		return(ft_putstr_fd("exit: too many arguments\n", 2), 1);
	else if (!ft_is_number(arr[1]) || is_out_of_range(arr[1]))
	{
		ft_putstr_fd("exit\n", 2);
		ft_putstr_fd("exit: numeric argument required\n", 2);
		free_before_exit(shell, NULL, NULL);
		exit(2);
	}
	else
	{
		exit_status = ft_atoll(arr[1]) % 256;
		free_before_exit(shell, NULL, NULL);
		ft_putstr_fd("exit\n", 2);
		exit(exit_status);
	}
}
