#include <stdio.h>

// int	check_op_para(char *line)
// {
// 	int		i;
// 	int		op_count;

// 	i = 0;
// 	op_count = 1;
// 	while (line[i] != '\0')
// 	{
// 		if (line[i] == '(')
// 		{
// 			i++;
// 			while (line[i] == '(')
// 			{
// 				i++;
// 				op_count++;
// 			}
// 			while (line[i] != '\0' && op_count != 0)
// 			{
// 				i++;
// 				if (line[i] == ')')
// 					op_count--;
// 			}
// 			if (line[i] == '\0')
// 				return (printf("syntax error near unexpected token `('\n"), 1);
// 		}
// 		else
// 			i++;
// 	}
// 	printf("op_count: %d\n", op_count);
// 	return (0);
// }

int	check_op_para(char *line)
{
	int		i;
	int		op_count;

	i = 0;
	op_count = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '')
		if (line[i] == '(')
		{
			i++;
			op_count++;
			while (op_count != 0)
			{
				if (line[i] == '(')
					op_count++;
				else if (line[i] == ')')
					op_count--;
				else if (line[i] == '\0')
					return (printf("syntax error near unexpected token `('\n"), 1);
				i++;
			}
		}
		else
			i++;
	}
	return (0);
}
int main()
{
	printf("check op para: %d\n", check_op_para("()(")); // 1
	printf("check op para: %d\n", check_op_para("()()")); // 0

	printf("check op para: %d\n", check_op_para("((())()")); // 1
	printf("check op para: %d\n", check_op_para("((())())")); // 0

	printf("check op para: %d\n", check_op_para("(()()")); // 1
	printf("check op para: %d\n", check_op_para("(()())")); // 0


	
}