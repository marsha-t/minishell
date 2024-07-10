#include <stdlib.h>
#include <stdio.h>

// void	free_char_dp(char **dp)
// {
// 	int	i;

// 	i = 0;
// 	while (dp[i])
// 	{
// 		printf("%d, %s\n", i, dp[i]);
// 		free(dp[i]);
// 		i++;
// 	}
// 	free(dp);
// }
void	free_char_dp(char **dp)
{
	while (*dp)
	{
		printf("%s\n", *dp);
		free(*dp);
		dp++;
	}
	free(dp);
}

int main()
{
	char **dp;
	dp = malloc(sizeof(char *) * 6);
	for (int i = 0; i < 5; i++)
	{
		dp[i] = malloc(2);
		dp[i][0] = 'a';
		dp[i][1] = '\0';
	}
	dp[5] = 0;
	for (int i = 0; i < 6; i++)
	{
		printf("%s\n", dp[i]);
	}
	printf("free\n");
	free_char_dp(dp);
	printf("finish\n");
}