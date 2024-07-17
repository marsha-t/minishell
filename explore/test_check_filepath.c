#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>

int	check_filepath(char *cmd)
{
	int	error;
	struct stat file_stat;

	error = access(cmd, X_OK);
	if (error == 0)
	{
		if (stat(cmd, &file_stat) == -1)
		{
			return (printf("error calling stat\n"), 1);
		}
		else if (S_ISDIR(file_stat.st_mode))
			return (printf("%s: Is a directory\n", cmd), 126);
		else
			return (0);
	}
	else if (errno == EACCES)
		return (printf("%s: Permission denied\n", cmd), 126);
	else if (errno == ENAMETOOLONG)
		return (printf("%s: File name too long\n", cmd), 126);
	else if (errno == ENOENT)
		return (printf("%s: No such file or directory\n", cmd), 127);
	else if (errno = ENOTDIR)
		return (printf("%s: Not a directory\n", cmd), 126);
	else
	{
		return (printf("error calling access\n"), 1);
	}
}

int main()
{
	// printf("%d\n", check_filepath("/workspaces/minishell/global.h")); // 0
	printf("%d\n", check_filepath("/workspaces/minishell/global.h/"));

}