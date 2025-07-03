/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 22:32:04 by sradosav          #+#    #+#             */
/*   Updated: 2025/06/23 16:51:04 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// void	ft_write_export(char *str)
// {
// 	int	i;

// 	write(1, "declare -x ", 11);
// 	i = 0;
// 	while (str[i] != 0 && str[i] != '=')
// 	{
// 		write(1, &str[i], 1);
// 		i++;
// 	}
// 	if (str[i] == '=')
// 	{
// 		write(1, &str[i], 1);
// 		write(1, "\"", 1);
// 		i++;
// 		while (str[i] != 0)
// 		{
// 			if (str[i] == '"')
// 				write(1, "\"", 1);
// 			write(1, &str[i], 1);
// 			i++;
// 		}
// 		write(1, "\"", 1);
// 	}
// 	write(1, "\n", 1);
// }

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

// void	ft_print_export(t_shell *shell)
// {
// 	t_envvar	*env_copy;
// 	t_envvar	*iter;

// 	env_copy = copy_env_list(shell, shell);
// 	ft_sort_env_list(env_copy);
// 	iter = env_copy;
// 	while (iter)
// 	{
// 		if (iter->exported == 1)
// 			ft_write_export(iter->var);
// 		iter = iter->next;
// 	}
// 	free_env_list(&env_copy);
// }

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

void	ft_export_vars(char **str, t_shell *shell)
{
	int		i;
	// int		var_len;
	// char	*var;
	// char	*value;

	i = 1;
	while (str[i])
	{
		if (!is_valid_identifier(str[i]))
		{
			printf("minishell: export: `%s': not a valid identifier\n", str[i]);
			shell->exit_status = 1;
		}
		else
			handle_valid_export(str[i], shell);
		i++;
	}
}

// COMME POUR LES AUTRES FONCTIONS, TOUJOURS ENVOYER export EN PREMIERE LIGNE
// DU TABLEAU
// void	ft_export(char **str, t_shell *shell, int in_pipeline)
// {
// 	int	str_size;

// 	str_size = count_strings(str);
// 	if (!str[1])
// 	{
// 		update_or_add("_", str[str_size - 1], shell, 0);
// 		ft_print_export(shell);
// 		shell->exit_status = 0;
// 	}
// 	else if (in_pipeline == 1)
// 	{
// 		shell->exit_status = 0;
// 		update_or_add("_", str[str_size - 1], shell, 0);
// 		ft_export_vars(str, shell);
// 	}
// 	else
// 		return ;
// }

void	ft_export(char **str, t_shell *shell, int in_pipeline, int fd_out)
{
	int	str_size;

	str_size = count_strings(str);
	if (!str[1])
	{
		update_or_add("_", str[str_size - 1], shell, 0);
		ft_print_export(shell, fd_out);
		shell->exit_status = 0;
	}
	else if (in_pipeline == 1)
	{
		shell->exit_status = 0;
		update_or_add("_", str[str_size - 1], shell, 0);
		ft_export_vars(str, shell);
	}
	else
		return ;
}

// int	main(int argc, char **argv, char **envp)
// {
// 	t_shell		*shell;
//     t_envvar    *env_copy;

// 	shell = malloc(sizeof(t_shell));
// 	if (!shell)
// 		return (1);
// 	shell->env = NULL;
// 	shell->exit_status = 0;
// 	char *test1[] = {"export", "Hello", "world", NULL};
// 	char *test2[] = {"export", NULL};

// 	shell->env = ft_env_to_list(envp);

// 	ft_export(test1, shell);	
// 	ft_export(test2, shell);

//     env_copy = shell->env;
// 	while (env_copy)
// 	{
// 		printf("%s\n%d\n\n", env_copy->var, env_copy->exported);
// 		env_copy = env_copy->next;
// 	}

// 	return 0;
// }
