#include "pipex.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

void	freee(char **s)
{
	int i;

	i = 0;
	while (s[i])
	{
		free(s[i]);
		i++;
	}
	free(s);
}

void	pipex_free(t_pipex p)
{
	freee(p.paths);
	close(p.in_file);
	close(p.out_file);
	close(p.pipe[0]);
	close(p.pipe[1]);
}

static int	sepcount(char const *s, char c)
{
	int	count;
	int	isword;

	count = 0;
	isword = 0;
	if (!s)
		return (0);
	while (*s)
	{
		if (*s != c && !isword)
		{
			count++;
			isword = 1;
		}
		else if (*s == c)
			isword = 0;
		s++;
	}
	return (count);
}

static int	next_word_len(char const *s, char c)
{
	int	count;

	count = 0;
	while (*s == c)
		s++;
	while (*s != c)
	{
		count++;
		s++;
	}
	return (count);
}

static char	*ft_next_str(char const **s, char c)
{
	char	*next_str;
	int		next_str_len;
	int		i;

	next_str_len = next_word_len(*s, c);
	next_str = malloc(sizeof(char) * (next_str_len + 1));
	if (!next_str)
		return (NULL);
	else
	{
		i = 0;
		while (**s == c)
			(*s)++;
		while (i < next_str_len)
		{
			next_str[i] = **s;
			(*s)++;
			i++;
		}
		next_str[i] = 0;
	}
	return (next_str);
}

static char	**free_split(char **splited, int i)
{
	while (i-- > 0)
		free(splited[i]);
	free(splited);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**split_tab;
	char	*next_str;
	int		i;
	int		size;

	size = sepcount(s, c);
	split_tab = malloc(sizeof(char *) * (size + 1));
	if (!split_tab || !s)
		return (NULL);
	else
	{
		i = 0;
		while (i < size)
		{
			next_str = ft_next_str(&s, c);
			if (next_str == NULL)
				break ;
			split_tab[i] = next_str;
			i++;
		}
		if (i < size)
			return (free_split(split_tab, i));
		split_tab[size] = NULL;
	}
	return (split_tab);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	while (n-- > 0)
	{
		if (*s1 != *s2 || !*s1 || !*s2)
			return (*(unsigned char *)s1 - *(unsigned char *)s2);
		s1++;
		s2++;
	}
	return (0);
}

char	*find_path(char *f, char **paths)
{
	int		i;
	char	*fpath;

	f = ft_strjoin(f, "/"); //voir les free (si f est free probleme)
	i = 0;
	while (paths[i])
	{
		if (!fpath)
			free(fpath);
		fpath = ft_strjoin(paths[i], f);
		if (access(fpath, F_OK))
		{
			free(f);
			return (fpath);
		}
		i++;
	}
	free(fpath);
	free(f);
	return (NULL);
}

char	**extract_paths(char **env)
{
	int i;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH", 4))
		i++;
	if (!env[i])
		perror("Impossible?");
	return (ft_split(env[i] + 5, ':'));
}

void	cmd1(char *f, t_pipex p, char **env)
{
	(void)f;
	char *str[] = { "/usr/bin/grep", "a", NULL };
	close(p.pipe[0]);
	dup2(p.in_file, STDIN_FILENO);
	close(p.in_file);
	dup2(p.pipe[1], STDOUT_FILENO);
	execve(str[0], &str[0], env);
}

void	cmd2(char *f, t_pipex p, char **env)
{
	(void)f;
	char *str[] = { "/usr/bin/wc", "-w", NULL };
	close(p.pipe[1]);
	dup2(p.out_file, STDOUT_FILENO);
	close(p.out_file);
	dup2(p.pipe[0], STDIN_FILENO);
	execve(str[0], &str[0], env);
}

int	main(int ac, char **av, char **env)
{
	t_pipex p;

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
	p.pid = fork();
	if (p.pid == -1)
		perror("FORK");
	if (p.pid == 0)
		cmd2(av[3], p, env);
	pipex_free(p);
	return 0;
}