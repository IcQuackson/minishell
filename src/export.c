/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaiao-r <abaiao-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 21:58:38 by abaiao-r          #+#    #+#             */
/*   Updated: 2023/05/05 19:52:49 by abaiao-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env	*sort_alphabet_env_list(t_env **head)
{
	t_env	*curr;
	int		swapped;

	swapped = 1;
	if (!head || !(*head)->next)
		return (*head);
	while (swapped)
	{
		swapped = 0;
		curr = *head;
		while (curr->next)
		{
			if (ft_strcmp(curr->var_name, curr->next->var_name) > 0)
			{
				swap_env_nodes(curr);
				swapped = 1;
			}
			curr = curr->next;
		}
	}
	return (*head);
}

int	export_error(char *input)
{
	printf("export: not an identifier: %s\n", input);
	return (NO_EXIT);
}

void	print_export(t_env **head)
{
	t_env	*node;
	t_env	*sorted_head;

	sorted_head = sort_alphabet_env_list(head);
	node = sorted_head;
	while (node != NULL)
	{
		printf("declare -x %s=\"%s\"\n", node->var_name, node->var_value);
		node = node->next;
	}
}

int	show_export(t_env **environment)
{
	print_export(environment);
	return (NO_EXIT);
}

int	export(char **input, int num_tokens, t_env **environment)
{
	int	i;

	if (num_tokens == 1)
		return (show_export(environment));
	if (num_tokens != 2 || input[1][0] == '_')
		return (NO_EXIT);
	if (input[1][0] == '_')
		return (NO_EXIT);
	if (!ft_isalpha(input[1][0]))
		return (export_error(input[1]));
	i = 0;
	while (input[1][i] && input[1][i] != '=')
	{
		if (!ft_isalnum(input[1][i]) && input[1][i] != '_')
			return (export_error(input[1]));
		i++;
	}
	if (ft_strchr(input[1], '='))
		ft_setenv(input[1], environment);
	//ft_putenv(input, env);
	return (NO_EXIT);
}
