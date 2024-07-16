#include <stdio.h>
#include <unistd.h>

char	*ft_strchr(const char *s, int c)
{
	while (*s != '\0')
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if (*s == (char)c)
		return ((char *)s);
	return (0);
}

int	count_dir(char *cmd)
{
	int	count;
	
	count = -1;
	while (*cmd)
	{
		if (ft_strchr(cmd, '/'))
		{
			cmd = ft_strchr(cmd, '/') + 1;
			count++;
			while (*cmd == '/')
			{
				cmd++;
			}
		}
		else
			cmd++;
	}
	if (count == -1)
		count = 0;
	return (count);
}

int main(void)
{
	// printf("%d\n", count_dir("./g")); // 0
	// printf("%d\n", count_dir("./g/a/s/d/f")); // 4
	// printf("%d\n", count_dir("./g//a")); // 1

	char *argv[] = {NULL};
    char *envp[] = {NULL};
	printf("%d\n", execve("./global.c/", argv, envp));

}
