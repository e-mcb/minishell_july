/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 22:31:52 by sradosav          #+#    #+#             */
/*   Updated: 2025/07/06 20:48:17 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// deux solutions pour gérer env avec arguments ou options:
// - soit faire le env et ignorer ce qu'il y a apres
// -soit envoyer une erreur s'il y a argument ou option
// dans cette version, je prefere envoyer une erreur.
// le sujet dit env sans option sans arguments.

int	ft_env(char **str, t_shell *shell, int fd_out)
{
	t_envvar	*env_copy;

	if (count_strings(str) > 1)
	{
		ft_putstr_fd("minishell: env: too many arguments or options\n", 2);
		// update_or_add("_", str[count_strings(str) - 1], shell, 0);
		return (1);
	}
	else
	{
		env_copy = shell->env;
		while (env_copy)
		{
			if (env_copy->exported == 1 && ft_strchr(env_copy->var, '=')
				&& ft_strncmp(env_copy->var, "_=", 2) != 0)
			{
				ft_putstr_fd(env_copy->var, fd_out);
				ft_putstr_fd("\n", fd_out);
			}
			env_copy = env_copy->next;
		}
		// update_or_add("_", str[count_strings(str) - 1], shell, 0);
		return (0);
	}
}
