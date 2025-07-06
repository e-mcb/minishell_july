/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 22:30:37 by sradosav          #+#    #+#             */
/*   Updated: 2025/07/06 20:47:48 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// ATTENTION : DOIT TOUJOURS ETRE APPELEE AVEC cd COMME
// PREMIERE CHAINE DU TABLEAU D ARGUMENTS
// utilise un buffer de taille 1024 pour 
// éviter de malloc
// si pas pile un  argument, erreur + mise à jour de $_
// si un argument, essaie le cd , mets a jour $_ 
// traite les ~, - etc.. comme des chemins normaux
// 25 LIGNES WESH :
int	ft_cd(char **str, t_shell *shell)
{
	char	*pwd;
	char	cwd[1024];

	pwd = ft_getenv("PWD", shell);
	shell->exit_status = 1;
	if (!str[1] || str[2])
	{
		if (!str[1])
			ft_putstr_fd("minishell: cd: missing argument\n", 2);
		else
			ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	if (chdir(str[1]) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	update_or_add("OLDPWD", pwd, shell, 0);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		update_or_add("PWD", cwd, shell, 0);
	return (0);
}
