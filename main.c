/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 11:16:07 by mzutter           #+#    #+#             */
/*   Updated: 2025/07/06 21:34:10 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	g_signal = 0;

void sigint_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static char	*prompt(t_shell *shell)
{
	char	*input;

	while (1)
	{
		input = readline("minishell>");
		if (input == NULL)
		{
			ft_putstr_fd("Goodbye\n", 2);
			ft_end_minishell(NULL, shell, NULL, NULL);
		}
		if (input[0] != '\0')
			add_history(input);
		if (input[0] == 0)
		{
			free(input);
			continue ;
		}
		if (ft_has_invalid_quotes(input))
		{
			ft_putstr_fd(OPEN_QUOTES, 2);
			free(input);
			return (NULL);
		}
		return (input);
	}
}

void	ft_set_shlvl(t_shell *shell, char *shlvl_str)
{
	int		value;
	char	*new_value;

	value = ft_atoi(shlvl_str);
	if (value >= 999)
	{
		ft_putstr_fd("minishell: warning: shell level (", 2);
		new_value = ft_itoa(value + 1);
		ft_putstr_fd(new_value, 2);
		free(new_value);
		ft_putstr_fd(") too high, resetting to 1\n", 2);
		value = 1;
	}
	else if (value < 0)
		value = 0;
	else if (value >= 0 && value < 999)
		value++;
	new_value = ft_itoa(value);
	update_or_add("SHLVL", new_value, shell, 1);
	free(new_value);
}

void	env_min(t_shell *shell)
{
	char	cwd[1024];
	char	*joined;

	if (getcwd(cwd, sizeof(cwd)) != 0)
	{
		joined = ft_strjoin("PWD=", cwd);
		if (!joined)
			ft_clean_exit(0, shell, 0, 0);
		add_env_var(&(shell->env), joined, 1, shell);
		free(joined);
	}
	else
	{
		perror("cwd init");
		ft_clean_exit(0, shell, 0, 0);
	}
	add_env_var(&(shell->env), PATH, 1, shell);
	add_env_var(&(shell->env), "OLDPWD", 1, shell);
	add_env_var(&(shell->env), "SHLVL=0", 1, shell);
}

static t_shell	*init_shell(t_shell *shell, char **envp)
{	
	char	*shlvl_str;
	
	shell = malloc(sizeof(t_shell));
	if (shell == NULL)
		ft_clean_exit(NULL, NULL, NULL, NULL);
	shell->env_arr = NULL;
	shell->splitted = NULL;
	shell->token = NULL;
	if (!envp[0])
		env_min(shell);
	else
		shell->env = ft_env_to_list(envp, shell);	
	shell->exit_status = 0;
	if (shell->env == NULL)
		ft_clean_exit(NULL, shell, NULL, NULL);
	shell->exec = NULL;
	shlvl_str = ft_getenv("SHLVL", shell);
	if (!shlvl_str)
		update_or_add("SHLVL", "1", shell, 1);
	else
		ft_set_shlvl(shell, shlvl_str);
	return (shell);
}

static void	ft_parsing(char *input, t_shell *shell)
{
	int		i;
	t_token	*tmp;

	i = 0;
	whitespace_to_space(input);
	shell->splitted = ft_split2(input, ' ');
	if (shell->splitted == NULL)
		ft_clean_exit(input, shell, NULL, NULL);
	free (input);
	while (shell->splitted[i])
	{
		tokenizer(shell, i);
		i++;
	}
	if (shell->splitted != NULL)
		ft_free_str_array(shell->splitted);
	refine_token_type(shell->token);
	tmp = shell->token;
	while (tmp)
	{
		if (ft_strchr(tmp->value, '\'') != NULL
		|| ft_strchr(tmp->value, '"') != NULL)
			tmp->in_quotes = true;
		tmp = tmp->next;
	}
	expand(shell);
	second_refine_token_type(shell->token);
	shell->splitted = NULL;
}

// static void	minishell_loop(t_shell *shell)
// {
// 	char	*input;
// 	// int		i;

// 	// i = 3;
// 	while (1)
// 	{
// 		input = prompt(shell);
// 		if (input == NULL)
// 			continue ;
// 		if (string_error(input))
// 		{
// 			free (input);
// 			continue ;
// 		}
// 		ft_parsing(input, shell);
// // 		if (token_error(shell) == 0)
// // 		{
// // 			create_exec(shell);
// // 			if(g_signal != SIGINT)
// // 			{
// // 				env_list_to_arr(shell);
// // 				exec_loop(shell);
// // 			}
// // 		}
// // 		free_list(&shell->token);
// // 		free_exec_list(&(shell->exec));
// // 		free(shell->exec);
// // 		ft_free_str_array(shell->env_arr);
// // 		while (i++ < 1023)
// // 			close(i);
// // 		shell->env_arr = NULL;
// // 		g_signal = 0;
// 	}
// }

static void	minishell_loop(t_shell *shell)
{
	char	*input;
	int		i;

	i = 3;
	while (1)
	{
		input = prompt(shell);
		if (input == NULL)
			continue ;
		if (string_error(input))
		{
			free (input);
			continue ;
		}
		ft_parsing(input, shell);
		if (token_error(shell) == 0)
		{
			create_exec(shell);
			if(g_signal != SIGINT)
			{
				env_list_to_arr(shell);
				exec_loop(shell);
			}
		}
		free_list(&shell->token);
		free_exec_list(&(shell->exec));
		free(shell->exec);
		ft_free_str_array(shell->env_arr);
		while (i++ < 1023)
			close(i);
		shell->env_arr = NULL;
		g_signal = 0;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	/*
	printf("%s:%d", __FILE__, __LINE__);
	ft_error(BRED"Erreur:"WHITE" %s:%d mon message erreur", __FILE__, __LINE__); //code ANSI pour les couleurs + exemple de fonction pour erreur (CTR+click dans le terminal VScode)
	CTRL + ` 			=> aller au terminal
	CTRL + SHIFT + E	=> aller a l'exploreur de fichiers
	*/
	signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN); 
	printf("Welcome to minishell\n");
	(void)argc;
	(void)argv;
	shell = NULL;
	shell = init_shell(shell, envp);
	minishell_loop(shell);
}
