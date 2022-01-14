#ifndef PIPEX_H
# define PIPEX_H

typedef struct s_pipex t_pipex;

struct s_pipex
{
	int	pipe[2];
	int	in_file;
	int	out_file;
	char **paths;
	int pid;
};

#endif