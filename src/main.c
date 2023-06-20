/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedgonca <pedgonca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 22:55:59 by quackson          #+#    #+#             */
/*   Updated: 2023/06/19 22:52:02 by pedgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_list(t_input *input)
{
	t_input	*temp;

	temp = input;
	while (temp != NULL)
	{
		printf("lst: %s  ", temp->token);
		printf("size:%ld len:%ld\n", sizeof(temp->token), ft_strlen(temp->token));
		temp = temp->next;
	}
}

void	print_array_of_strings(char ** strings)
{
	int	i;

	i = 0;
	while (strings[i])
	{
		printf("string[%d]: %s  ", i, strings[i]);
		printf("size:%ld len:%ld\n", sizeof(strings[i]), ft_strlen(strings[i]));
		i++;
	}
}

struct s_global_minishell	g_minishell;

int	main(int argc, char **argv, char **env)
{
	char		*input;
	int			response;
	t_minishell	*minishell;

	 if (argc > 1)
        return 0;
	(void)argv;
	minishell = init_minishell(env);
	g_minishell.minishell = minishell;
	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
		g_minishell.in_command = 0;
		input = readline("\033[1;33mminishell$ \033[0m");
		signal(SIGQUIT, sig_handler);
		response = validate_and_load_data(minishell, input);
		if (response == INVALID)
			continue ;
		minishell->tokens = create_token_array_2d(minishell->input, minishell);
		if (!minishell->tokens)
		{
			free_input_resources(minishell);
			continue ;
		}
		exe_commands(minishell);
		free_input_resources(minishell);
		g_minishell.in_command = 0;
	}
	free_minishell(minishell);
	return (0);
}
