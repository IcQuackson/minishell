/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaiao-r <abaiao-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 15:24:17 by quackson          #+#    #+#             */
/*   Updated: 2023/05/22 14:49:49 by abaiao-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Para ele aceitar o comando pedido, precisa de ir pesquisar no caminho do bash
se esse comando existe e se tem permissao para utiliza-lo
*/
char	*find_executable(char *cmd)
{
	char	*path;
	char	*path_end;
	char	*executable;

	path = getenv("PATH");
	if (!path)
		return (NULL);
	while (*path)
	{
		path_end = ft_strchr(path, ':');
		if (!path_end)
			path_end = path + ft_strlen(path);
		executable = malloc(path_end - path + ft_strlen(cmd) + 2);
		ft_strncpy(executable, path, path_end - path);
		executable[path_end - path] = '/';
		ft_strcpy(executable + (path_end - path) + 1, cmd);
		printf("executable: %s\n", executable);
		if (!access(executable, X_OK))
			return (executable);
		free(executable);
		path = path_end;
		if (*path == ':')
			path++;
	}
	return (NULL);
}

/* Isto e uma tentative de pegar num conjunto de strings
 e extrair aquelas que estao antes do proximo simbolo "|" ou ">", etc
 Nao esta funcional
 */
char	**get_cmd(char **input, char c)
{
	int		i;
	int		n;
	char	**cmd;

	i = 0;
	while (input[i] && input[i][0] != c)
		i++;
	if (!input[i])
		return (input);
	n = i;
	i = 0;
	cmd = malloc(n * sizeof(char *));
	if (!cmd)
		return (NULL);
	while (input[i] && input[i][0] != c)
	{
		cmd[i] = input[i];
		i++;
	}
	return (cmd);
}

/* Tentei por isto a funcionar mas depois explico o problema */
/* void	execute_pipe(char **args)
{
	int		pipefd[2];
	pid_t	pid1, pid2;
	char	**args_cpy;
	char	**cmd1;
	char	**cmd2;

	args_cpy = args;
	cmd1 = get_cmd(args);
	cmd1 = 1;
    if (pipe(pipefd) < 0) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid1 = fork();
    if (pid1 < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        // Child 1 code
        close(pipefd[0]); // Close unused read end
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to the write end of the pipe
        close(pipefd[1]); // Close write end
        char *cmd1 = find_executable(args[0]);
        if (cmd1 == NULL) {
            fprintf(stderr, "Command not found: %s\n", args[0]);
            exit(EXIT_FAILURE);
        }
        execve(cmd1, {arg[0], NULL}, NULL);
        perror(cmd1);
        exit(EXIT_FAILURE);
    }

    pid2 = fork();
    if (pid2 < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0) {
        // Child 2 code
        close(pipefd[1]); // Close unused write end
        dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to the read end of the pipe
        close(pipefd[0]); // Close read end
        char *cmd2 = find_executable(args[2]);
        if (cmd2 == NULL) {
            fprintf(stderr, "Command not found: %s\n", args[2]);
            exit(EXIT_FAILURE);
        }
        execve(cmd2, args + 2, NULL);
        perror(cmd2);
        exit(EXIT_FAILURE);
    }

    // Parent code
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
} */

int	exe_shell_cmd(char **args)
{
	pid_t	pid;
	int		status;

	if (!args)
	{
		printf("NULL shell cmd\n");
		return (NO_EXIT);
	}
	pid = fork();
	if (pid == 0)
	{
        execve(args[0], args, NULL);
		//execve("/bin/bash", (char *[]){"/bin/bash", "-c", cmd, NULL}, NULL);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else
	{
		// parent process
		do {
			waitpid(pid, &status, WUNTRACED);
		}
		while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return (NO_EXIT);
}

int	exe_cmd(char **parsed, char *input, int num_tokens, t_env **environment)
{
	(void) input;
	if (!parsed || !*parsed || num_tokens <= 0)
		return (1);
	if (ft_strcmp(parsed[0], "echo") == 0)
		return (echo(parsed, num_tokens));
	else if (ft_strcmp(parsed[0], "cd") == 0)
		return (change_dir(parsed, num_tokens));
	else if (ft_strcmp(parsed[0], "pwd") == 0)
		return (pwd());
	else if (ft_strcmp(parsed[0], "export") == 0)
		return (export(parsed, num_tokens, environment));
	else if (ft_strcmp(parsed[0], "unset") == 0)
		return (ft_unset(parsed, num_tokens, environment));
	else if (ft_strcmp(parsed[0], "env") == 0)
		return (show_env(environment));
	else if (ft_strcmp(parsed[0], "exit") == 0)
		return (EXIT);
	else
		return (exe_shell_cmd(parsed));
}

/* Apenas executa um comando. Ainda nao aceita redirecionamento de input/output */
void	exe_executable(char **input)
{
	pid_t	pid;
	int		status;
	char	*executable;

	// Procura o comando
	executable = find_executable(input[0]);
	if (executable == NULL)
	{
		fprintf(stderr, "Command not found: %s\n", input[0]);
		return ;
	}
	//execute_pipe(input);
	pid = fork();
	if (pid == 0)
	{
		// child process
		execve(executable, input, NULL);
		// if execve returns, an error occurred
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else
	{
		// parent process
		do {
			waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
}
