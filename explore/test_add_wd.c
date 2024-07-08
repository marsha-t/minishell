#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

void	ft_putstr_fd(char *s, int fd)
{
	if (s)
	{
		write(fd, s, ft_strlen(s));
	}
}



char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*ptr;
	size_t	tot_len;
	int		i;

	if ((!s1) || (!s2))
		return (0);
	tot_len = ft_strlen(s1) + ft_strlen(s2) + 1;
	ptr = (char *)malloc(sizeof(char) * tot_len);
	if (!ptr)
		return (0);
	i = 0;
	while (*s1)
		ptr[i++] = *(s1)++;
	while (*s2)
		ptr[i++] = *(s2)++;
	ptr[i] = '\0';
	return (ptr);
}

/*	has_current_wd returns the position of the colon representing current working directory
	- if current working directory isn't listed, return -1 */
int	has_current_wd(char *path)
{
	int i;
	
	if (path[0] == ':')
		return (0);
	else if (path[ft_strlen(path) - 1] == ':')
		return (ft_strlen(path) - 1);
	else
	{
		i = 0;
		while (path[i])
		{
			if (path[i] == ':' && path[i + 1] == ':')
				return (i);
			i++;
		}
		return (-1);
	}

}
/*	add_current_wd adds the current working directory to path 
	and do so in the correct order */
char *add_current_wd(char *path, int i)
{
	char *dir;
	int	size;
	char *new_path;
	int	j;
	char	*temp;
	int	k;

	dir = 0;
	size = 1024;
	while (1)
	{
		dir = malloc(sizeof(char) *size);
		if (!dir)
			return (err_printf("minishell: malloc error creating dir\n"), NULL);
		if (getcwd(dir, size) != NULL)
			break;
		else if (errno == ERANGE)
			size *= 2;
		else
		{
			free(dir);
			return(err_printf("minishell: error calling getcwd\n"), NULL); // terminate shell
		}
	}
	if (i == 0)
	{
		temp = ft_strjoin(dir, ":");
		new_path = ft_strjoin(temp, path);
		free(temp);
	}
	else if (i == ft_strlen(path) - 1)
	{
		temp = ft_strjoin(path, ":");
		new_path = ft_strjoin(temp, dir);
		free(temp);
	}
	else
	{
		new_path = malloc(sizeof(char) * (ft_strlen(path) + ft_strlen(dir) + 2));
		j = 0;
		while (j <= i)
		{
			new_path[j] = path[j];
			j++;
		}
		k = 0;
		while (dir[k])
			new_path[j++] = dir[k++];
		i++;
		while (path[i])
			new_path[j++] = path[i++];
	}
	free(path);
	free(dir);
	return (new_path);
}

char	*ft_strdup(const char *src)
{
	char		*ptr;
	size_t		n;
	int			i;

	n = ft_strlen(src);
	ptr = (char *)malloc(sizeof(char) * (n + 1));
	if (!ptr)
		return (0);
	i = -1;
	while (src[++i] != '\0')
		ptr[i] = src[i];
	ptr[i] = '\0';
	return (ptr);
}

int main()
{
	char *value = ft_strdup(":/usr/local:/usr/bin");
	// printf("%d\n", has_current_wd(value));

	value = add_current_wd(value, has_current_wd(value));
	printf("%s\n", value);
}