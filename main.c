/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sradosav <sradosav@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 11:16:07 by mzutter           #+#    #+#             */
/*   Updated: 2025/07/03 18:38:06 by sradosav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	g_status = 0;

void sigint_handler(int sig)
{
    (void)sig;
    if (g_status == 0)
    {
        write(1, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
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
			ft_clean_exit(NULL, shell, NULL, NULL);
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

static t_shell	*init_shell(t_shell *shell, char **envp)
{
	shell = malloc(sizeof(t_shell));
	if (shell == NULL)
		ft_clean_exit(NULL, NULL, NULL, NULL);
	shell->env_arr = NULL;
	shell->splitted = NULL;
	shell->token = NULL;
	shell->env = ft_env_to_list(envp, shell);
	shell->exit_status = 0;
	if (shell->env == NULL)
		ft_clean_exit(NULL, shell, NULL, NULL);
	shell->exec = NULL;
	return (shell);
}

static void	ft_parsing(char *input, t_shell *shell)
{
	int		i;

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
	expand(shell);
	second_refine_token_type(shell->token);
	shell->splitted = NULL;
}

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
		ft_parsing(input, shell);
		create_exec(shell);
		if(g_status != -1)
		{
			env_list_to_arr(shell);
			exec_loop(shell);
		}
		
		free_list(&shell->token);
		free_exec_list(&(shell->exec));
		free(shell->exec);
		ft_free_str_array(shell->env_arr);
		while (i++ < 1023)
			close(i);
		shell->env_arr = NULL;
		g_status = 0;
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
