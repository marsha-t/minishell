#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

char	*ft_getcwd()
{
	char *dir;
	int	size;
	
	dir = 0;
	size = 1024;
	while (1)
	{
		dir = malloc(sizeof(char) *size);
		if (!dir)
			return (NULL);
		if (getcwd(dir, size) != NULL)
			break;
		else if (errno == ERANGE)
			size *= 2;
		else
		{
			free(dir);
			return(NULL);
		}
	}
	return (dir);
}

int main() {
    // char *path = "C:\\Users\\marsh\\OneDrive\\Desktop\\minishell\\execute";
    char *path = "..\\execute";

    printf("Before: %s\n", ft_getcwd());
    
    if (chdir(path) == 0) {
        printf("Changed directory to %s\n", path);
    } else {
        perror("chdir() failed");
    }
    printf("After: %s\n", ft_getcwd());
	return 0;
}
