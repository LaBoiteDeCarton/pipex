#include "pipex.h"

void	cmd(char *f, t_pipex *p, int fdin, int fdout)
{
	char	*tmp;
	char	**str;

	str = ft_split(f, ' ');
	tmp = str[0];
	str[0] = find_path(tmp, p->paths);
	free(tmp);
	dup2(fdin, STDIN_FILENO);
	dup2(fdout, STDOUT_FILENO);
	close(fdin);
	close(fdout);
	execve(str[0], &str[0], p->env);
}

// void	cmd1(char *f, t_pipex p, char **env)
// {
// 	char	*tmp;
// 	char	**str;

// 	str = ft_split(f, ' ');
// 	tmp = str[0];
// 	str[0] = find_path(tmp, p.paths);
// 	free(tmp);
// 	close(p.pipe[0]);
// 	dup2(p.in_file, STDIN_FILENO);
// 	close(p.in_file);
// 	dup2(p.pipe[1], STDOUT_FILENO);
// 	execve(str[0], &str[0], env);
// }

// void	cmd2(char *f, t_pipex p, char **env)
// {
// 	char	*tmp;
// 	char	**str;

// 	str = ft_split(f, ' ');
// 	tmp = str[0];
// 	str[0] = find_path(tmp, p.paths);
// 	free(tmp);
// 	close(p.pipe[1]);
// 	dup2(p.out_file, STDOUT_FILENO);
// 	close(p.out_file);
// 	dup2(p.pipe[0], STDIN_FILENO);
// 	execve(str[0], &str[0], env);
// }

void pipex(int ac, char **av, t_pipex *p)
{
	if (pipe(p->pipe) == -1)
			exit(EXIT_FAILURE);
	if (ac == 0)
	{
		dup2(p->out_file, p->pipe[1]);
		close(p->out_file);
	}
	p->pid = fork();
	if (p->pid == -1)
		perror("FORK");
	if (p->pid == 0)
	{
		close(p->pipe[0]);
		printf("%s\n", *av);
		cmd(*av, p, p->oldout, p->pipe[1]);
	}
	close(p->pipe[1]);
	p->oldout = p->pipe[0];
	if (ac > 0)
		pipex(ac - 1, av + 1, p);
	else
		close(p->pipe[0]);
}

int	main(int ac, char **av, char **env)
{
	t_pipex	p;

	p.in_file = open(av[1], O_RDONLY);
	p.out_file = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (p.in_file == -1)
		perror("FILE");
	if (p.out_file == -1)
		perror("FILE");
	p.paths = extract_paths(env);
	p.env = env;
	pipex(ac - 4, av + 2, &p);
	pipex_free(p);
	return (0);
}
