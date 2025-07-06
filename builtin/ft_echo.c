/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 22:31:46 by sradosav          #+#    #+#             */
/*   Updated: 2025/07/06 20:48:03 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Renvoie 1 si l on a une chaine du type -n ou -nnnnn ou -nnnnnnnnn ...
// Sinon renvoie 0. 
// Teste les premiers arguments passés à ft_echo pour savoir si il y a 
// le -n ou pas. 
// -n enlève le \n du résultat de ft_echo
int	is_n_flag(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	if (s[i] != '-')
		return (0);
	i++;
	while (s[i])
	{
		if (s[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	ft_echo(char **str, t_shell *shell, int fd_out)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (str[i] && is_n_flag(str[i]))
	{
		newline = 0;
		i++;
	}
	//printf a check avec perror pour le cas de /dev/full par example
	while (str[i])
	{
		ft_putstr_fd(str[i], fd_out);
		if (str[i + 1])
			ft_putstr_fd(" ", fd_out);
		i++;
	}
	if (newline)
		ft_putstr_fd("\n", fd_out);
	// update_or_add("_", str[i - 1], shell, 0);
	shell->exit_status = 0;
	return (0);
}
