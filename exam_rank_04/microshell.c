#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define STDIN 0
#define STDOUT 1

static void	ft_print(char *str)
{
	while (*str)
		write(2, str++, 1);
}

static int	ft_cd(char **argv, int count)
{
	if (count != 2)
		return (ft_print("error: cd: bad arguments\n"), 1);
	else if (chdir(argv[1]) == -1)
		return (ft_print("error: cd: cannot change directory to "), ft_print(argv[1]), ft_print("\n"), 1);
	return (0);
}

static int	ft_exec(char **argv, char **env, int count)
{
	int	status;
	int	fds[2];
	int	pid;
	int	pipes;

	pipes = (argv[count] && strcmp(argv[count], "|") == 0);
	if (pipes && pipe(fds) == -1)
		return (ft_print("error: fatal\n"), 1);
	pid = fork();
	if (pid == 0)
	{
		argv[count] = 0;
		if (pipes && (dup2(fds[STDOUT], STDOUT) == -1 || close(fds[STDIN]) == -1 || close(fds[STDOUT]) == -1))
			return (ft_print("error: fatal\n"), 1);
		execve(*argv, argv, env);
		return (ft_print("error: cannot execute "), ft_print(*argv), ft_print("\n"), 1);
	}
	waitpid(pid, &status, 0);
	if (pipes && (dup2(fds[STDIN], STDIN) == -1 || close(fds[STDIN]) == -1 || close(fds[STDOUT]) == -1))
		return (ft_print("error: fatal\n"), 1);
	return (WIFEXITED(status) && WEXITSTATUS(status));
}

int	main(int argc, char **argv, char **env)
{
	(void)argc;
	int	status;
	int	count;
	
	status = 0;
	while (*argv && *(argv + 1))
	{
		argv++;
		count = 0;
		while (argv[count] && strcmp(argv[count], "|") && strcmp(argv[count], ";"))
			count++;
		if (strcmp(*argv, "cd") == 0)
			status = ft_cd(argv, count);
		else if (count)
			status = ft_exec(argv, env, count);
		argv += count;
	}
	return (status);
}
