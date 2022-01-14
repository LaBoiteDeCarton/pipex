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
	int	pipe[2];
	int	in_file;
	int	out_file;
	char **paths;
	int pid;
};

char	**extract_paths(char **env);
char	*find_path(char *f, char **paths);
void	pipex_free(t_pipex p);

#endif