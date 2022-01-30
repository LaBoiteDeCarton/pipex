#include "pipex.h"
#include <string.h>


void	cmd(char *f, t_pipex *p)
{
	char	*tmp;
	char	**str;

	str = ft_split(f, ' ');
	tmp = str[0];
	str[0] = find_path(tmp, p->paths);
	if (!str[0])
	{
		ft_putstr_fd("Command not found: ", 2);
		ft_putendl_fd(f, 2);
		freee(str);
		exit(EXIT_FAILURE);
	}
	execve(str[0], &str[0], p->env);
	perror(f);
	freee(str);
	exit(EXIT_FAILURE);
}

void	fork_pipex(int ac, char **av, t_pipex *p)
{
	int	fd[2];
	int	pid;

	if (pipe(fd) == -1)
		exit(EXIT_FAILURE);
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	if (pid == 0)
	{
		close(fd[0]);
		if (ac > 2)
			dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		cmd(*av, p);
	}
	close(fd[1]);
	if (ac > 2)
		dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	waitpid(pid, NULL, 0);
}

void	set_stdout(char *file, t_pipex *p)
{
	int fd_out;

	if (p->heredoc)
		fd_out = open(file, O_WRONLY | O_CREAT | O_APPEND, 0777);
	else	
		fd_out = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd_out == -1)
	{
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(file, 2);
		exit(EXIT_FAILURE);
	}
	dup2(fd_out, STDOUT_FILENO);
	close(fd_out);
}

void	set_stdin(char **av, t_pipex *p)
{
	p->heredoc = !ft_strncmp(*av, "here_doc", 9);
	if (p->heredoc)
		fork_heredoc(av[1]);
	else
	{
		p->fd_in = open(*av, O_RDONLY);
		if (p->fd_in == -1)
		{
			perror(*av);
			exit(EXIT_FAILURE);
		}
		if (dup2(p->fd_in, STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(p->fd_in);
	}
}

int	main(int ac, char **av, char **env)
{
	t_pipex p;

	if (ac-- < 4)
		return (0);
	set_stdin(++av, &p);
	av++;
	ac--;
	if (p.heredoc || p.fd_in == -1)
		av++;
	if (p.heredoc || p.fd_in == -1)	
		ac--;
	set_stdout(av[ac - 1], &p);
	p.paths = extract_paths(env);
 	p.env = env;
	while (ac > 1)
	{
		ft_putstr_fd("while\n", 2);
		fork_pipex(ac--, av++, &p);
	}
	pipex_free(p);
	return (0);
}
// void	cmd(char *f, t_pipex *p, int fdin, int fdout)
// {
// 	char	*tmp;
// 	char	**str;

// 	str = ft_split(f, ' ');
// 	tmp = str[0];
// 	str[0] = find_path(tmp, p->paths);
// 	free(tmp);
// 	if (fdin != STDIN_FILENO)
// 	{
// 		dup2(fdin, STDIN_FILENO);  //dup2 peut echouer, faire un catch error ici
// 		close(fdin);
// 	}
// 	dup2(fdout, STDOUT_FILENO);
// 	close(fdout);
// 	execve(str[0], &str[0], p->env);
// 	ft_putstr_fd("command not found: ", 2);
// 	ft_putendl_fd(f, 2);
// 	exit(EXIT_FAILURE); //on termine le fork

// 		 //est-ce que le execve fait free le str[0] ???
// 	//il faudrait surement faire le ftstrjoin avant le fork? commme ca on peut free apres le execve dans le process parent.
// }

// void pipex(int ac, char **av, t_pipex *p)
// {
// 	if (pipe(p->pipe) == -1)
// 			exit(EXIT_FAILURE);
// 	if (ac == 0)
// 	{
// 		dup2(p->out_file, p->pipe[1]);
// 		close(p->out_file);
// 	}
// 	p->pid = fork();
// 	if (p->pid == -1)
// 		perror("FORK");
// 	if (p->pid == 0)
// 	{
// 		close(p->pipe[0]);
// 		if (p->oldout == -1)
// 			exit(EXIT_SUCCESS);
// 		cmd(*av, p, p->oldout, p->pipe[1]);
// 	}
// 	close(p->pipe[1]);
// 	p->oldout = p->pipe[0];
// 	if (ac > 0)
// 		pipex(ac - 1, av + 1, p);
// 	else
// 		close(p->pipe[0]);
// 	waitpid(p->pid, NULL, 0);
// }

// void	open_files(int ac, char **av, t_pipex *p)
// {
// 	if (!p->heredoc)
// 	{
// 		p->oldout = open(av[1], O_RDONLY);
// 		if (p.oldout == -1)
// 		{
// 			ft_putstr_fd(strerror(errno), 2);
// 			ft_putstr_fd(": ", 2);
// 			ft_putendl_fd(av[1], 2);
// 		}
// 	}

// 	p.out_file = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
// 	if (p.out_file == -1)
// 	{
// 		ft_putstr_fd(strerror(errno), 2);
// 		ft_putstr_fd(": ", 2);
// 		ft_putendl_fd(av[ac - 1], 2);
// 		exit(EXIT_FAILURE);
// 	}	
// }

// int	main(int ac, char **av, char **env)
// {
// 	t_pipex	p;
// 	(void)env;

// 	if (ac < 5)
// 		exit(0);
// 	p.heredoc = !ft_strncmp(av[1], "here_doc", 9);
// 	if (p.heredoc)
// 	{
// 		fork_heredoc(av[2], &p);
// 		ac--;
// 		av++;
// 	}
	
// 	p.paths = extract_paths(env);
// 	p.env = env;
// 	pipex(ac - 4, av + 2, &p);
// 	pipex_free(p);
// 	return (0);
// }
