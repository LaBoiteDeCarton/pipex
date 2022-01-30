#include "pipex.h"

void	heredoc(char *limiter, int fd)
{
	char	*line;

	ft_putstr_fd("heredoc> ", STDOUT_FILENO);
	while (get_next_line(0, &line))
	{
		if (line && !ft_strncmp(limiter, line, ft_strlen(limiter) + 1)) //len + 1? pour prendre en compte le NULL?
		{
			free(line);
			exit(EXIT_SUCCESS);
		}
		ft_putstr_fd(line, fd); //ne devrait pas poser probleme
		ft_putstr_fd("\n", fd);
		free(line);
		ft_putstr_fd("heredoc> ", STDOUT_FILENO);
	}
}

void	fork_heredoc(char *limiter)
{
	int	fd[2];
	int pid;

	if (pipe(fd) == -1)
		exit(EXIT_FAILURE);
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	if (pid == 0)
	{
		close(fd[0]);
		heredoc(limiter, fd[1]);
	}
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	waitpid(pid, NULL, 0);
}