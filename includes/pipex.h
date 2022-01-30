#ifndef PIPEX_H
# define PIPEX_H

#include "libft.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct s_pipex t_pipex;

struct s_pipex
{
	char	**env;
	int		fd_in;
	int		oldout;
	char	**paths;
	int		heredoc;
};

char	**extract_paths(char **env);
char	*find_path(char *f, char **paths);
void	freee(char **s);
void	pipex_free(t_pipex p);
void	fork_heredoc(char *limiter);

#endif