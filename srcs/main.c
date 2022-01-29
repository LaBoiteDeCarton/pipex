#include "pipex.h"
#include <string.h>

void	cmd(char *f, t_pipex *p, int fdin, int fdout)
{
	char	*tmp;
	char	**str;

	str = ft_split(f, ' ');
	tmp = str[0];
	str[0] = find_path(tmp, p->paths);
	free(tmp);
	dup2(fdin, STDIN_FILENO);  //dup2 peut echouer, faire un catch error ici
	dup2(fdout, STDOUT_FILENO);
	close(fdin);
	close(fdout);
	execve(str[0], &str[0], p->env);
	ft_putstr_fd("command not found: ", 2);
	ft_putendl_fd(f, 2);
	exit(EXIT_FAILURE); //on termine le fork

		 //est-ce que le execve fait free le str[0] ???
	//il faudrait surement faire le ftstrjoin avant le fork? commme ca on peut free apres le execve dans le process parent.
}

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

	if (ac < 5)
		exit (0);
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
