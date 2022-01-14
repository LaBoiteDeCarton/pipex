#include "pipex.h"

void	cmd1(char *f, t_pipex p, char **env)
{
	char	*tmp;
	char	**str;

	str = ft_split(f, ' ');
	tmp = str[0];
	str[0] = find_path(tmp, p.paths);
	free(tmp);
	close(p.pipe[0]);
	dup2(p.in_file, STDIN_FILENO);
	close(p.in_file);
	dup2(p.pipe[1], STDOUT_FILENO);
	execve(str[0], &str[0], env);
}

void	cmd2(char *f, t_pipex p, char **env)
{
	char	*tmp;
	char	**str;

	str = ft_split(f, ' ');
	tmp = str[0];
	str[0] = find_path(tmp, p.paths);
	free(tmp);
	close(p.pipe[1]);
	dup2(p.out_file, STDOUT_FILENO);
	close(p.out_file);
	dup2(p.pipe[0], STDIN_FILENO);
	execve(str[0], &str[0], env);
}

int	main(int ac, char **av, char **env)
{
	t_pipex	p;

	if (ac < 5)
		perror("ARG");
	p.in_file = open(av[1], O_RDONLY);
	p.out_file = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (p.in_file == -1)
		perror("FILE");
	if (p.out_file == -1)
		perror("FILE");
	if (pipe(p.pipe) == -1)
		perror("PIPE");
	p.paths = extract_paths(env);
	p.pid = fork();
	if (p.pid == -1)
		perror("FORK");
	if (p.pid == 0)
		cmd1(av[2], p, env);
	wait(0);
	p.pid = fork();
	if (p.pid == -1)
		perror("FORK");
	if (p.pid == 0)
		cmd2(av[3], p, env);
	pipex_free(p);
	return (0);
}
