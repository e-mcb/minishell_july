/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 21:55:23 by mzutter           #+#    #+#             */
/*   Updated: 2025/06/15 11:12:50 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_has_invalid_quotes(const char *str)
{
	int		i;
	bool	in_quotes;
	char	opening_quote;

	i = 0;
	in_quotes = false;
	opening_quote = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '\"'))
		{
			if (!in_quotes)
			{
				in_quotes = true;
				opening_quote = str[i];
			}
			else if (str[i] == opening_quote)
				in_quotes = false;
		}
		i++;
	}
	return (in_quotes);
}

int	is_str_digit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

void	ft_parsing(char *input, t_shell *shell)
{
	int		i;
	t_token	*tmp;

	i = -1;
	//whitespace_to_space(input);
	shell->splitted = ft_split2(input);
	if (shell->splitted == NULL)
		ft_clean_exit(input, shell, NULL, NULL);
	free (input);
	while (shell->splitted[++i])
		tokenizer(shell, i);
	if (shell->splitted != NULL)
		ft_free_str_array(shell->splitted);
	refine_token_type(shell->token);
	tmp = shell->token;
	while (tmp)
	{
		if (ft_strchr(tmp->value, '\'') || ft_strchr(tmp->value, '"'))
			tmp->in_quotes = true;
		tmp = tmp->next;
	}
	expand(shell);
	second_refine_token_type(shell->token);
	shell->splitted = NULL;
}
