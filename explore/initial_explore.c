/* Marsha's initial exploration*/

// #include "minishell.h"
// int	main(void)
// {
// 	char	*input;

// 	while (1)
// 	{
// 		input = readline("minishell $ ");
// 		if (input != NULL && input[0] !='\0') // if readline is successful & input is not a blank line
// 		{
// 			add_history(input);
// 			free(input); 
// 		}
// 	} 
// 	rl_clear_history();
// 	return (0);
// }

#include <stdio.h>
int main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	int i = 0;
	while (envp[i] != NULL)
	{
		printf("%s\n", envp[i++]);
	}
}