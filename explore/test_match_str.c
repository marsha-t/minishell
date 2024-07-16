#include <stdio.h>

int	match_pattern_str(char *pattern, char *str)
{
	while (*pattern)
	{
		if (*pattern == *str)
		{
			pattern++;
			str++;
		}
		else if (*pattern == '*')
		{
			pattern++;
			if (*pattern == '\0')
				return (0);
			while (*str)
			{
				if (match_pattern_str(pattern, str) == 0)
					return (0);
				str++;
			}
			return (1);
		}
		else
			return (1);
	}
	if (*str == '\0')
		return (0);
	else
		return (1);
}

int main()
{
	printf("%d\n", match_pattern_str("expansion*", "expansion")); // 0
	
	// printf("%d\n", match_pattern_str("e*p*", "expansion")); // 0
	// printf("%d\n", match_pattern_str("e*p*", "ep")); // 0
	// printf("%d\n", match_pattern_str("e*p", "ep")); // 0
	// printf("%d\n", match_pattern_str("e*p*n", "expansion")); // 0
	// printf("%d\n", match_pattern_str("e*p*n*", "expansion")); // 0
	// printf("%d\n", match_pattern_str("e*p", "expansion")); // 1
}