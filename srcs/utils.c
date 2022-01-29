#include "pipex.h"

static void	freee(char **s)
{
	int	i;

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
	close(p.pipe[0]);
	close(p.pipe[1]);
}

char	*find_path(char *f, char **paths)
{
	int		i;
	char	*fpath;

	if (access(f, F_OK) == 0)
		return (f);
	f = ft_strjoin("/", f); //leaks detected!
	i = 0;
	while (paths[i])
	{
		if (!fpath)
			free(fpath);
		fpath = ft_strjoin(paths[i], f);
		if (access(fpath, F_OK) == 0)
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
	int	i;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH", 4))
		i++;
	if (!env[i])
		return (NULL); //ne devrait arriver que is unset PATH?
	return (ft_split(env[i] + 5, ':'));
}
