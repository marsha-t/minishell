#include <stdio.h>

/*	is_quote returns 0 if c is a single or double quote */
int	is_quote(char c)
{
	if (c == '\'' || c == '\"')
		return (0);
	return (1);
}
/*	is_valid_varstart checks whether the char after $ is valid
	- valid if c is a letter or underscore: returns 0 */
int	is_valid_varstart(char c)
{
	if (c >= 'a' && c <= 'z')
		return (0);
	if (c >= 'A' && c <= 'Z')
		return (0);
	if (c == '_')
		return (0);
	else
		return (1);
}
/*	contain_var checks whether str contains a variable expansion 
	i.e., a $ followed by valid variable name character 
	- if $ is followed by a starting quote, it is counted as a variable expansion (that expands into empty str)
	- if $ followed by number of special character, it doesn't count as variable expansion
	- if there is variable expansion, return (0)
	flow:
	- if there is a single quote, skip everything until the end of single quote
*/
int contain_var(char *str)
{
	int i;

	if (!str)
		return (1);
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == 39)
		{
			i++;
			while (str[i] != '\0' && str[i] != 39)
				i++;
		}
		else if (str[i] == '\"')
		{
			i++;
			while (str[i] != '\0' && str[i] != '\"')
			{
				if (str[i] == '$' && is_valid_varstart(str[i + 1]) == 0)
					return (0);
				i++;
			}
		}
		else if (str[i] == '$' && is_quote(str[i + 1]) == 0)
			return (0);
		else if (str[i] == '$' && is_valid_varstart(str[i + 1]) == 0)
			return (0);
		i++;
	}
	return (1);
}

int main()
{
	printf("%d\n", contain_var("\"$var\"")); // "$var": 0
	printf("%d\n", contain_var("\'$var\'")); // '$var': 1
	printf("%d\n", contain_var("\'$var\'$var")); // '$var'$var: 0
	printf("%d\n", contain_var("\'$var\'\"$var\"")); // '$var'"$var"0
	printf("%d\n", contain_var("$\"abc\"")); // $"abc": 0
	printf("%d\n", contain_var("$\'abc\'")); // $'abc': 0
	printf("%d\n", contain_var("\"abc$\"")); // "abc$": 1
	printf("%d\n", contain_var("\'abc$\'")); // 'abc$': 1
	printf("%d\n", contain_var("$\"$path\"")); // $"$path": 0 
	printf("%d\n", contain_var("$\"$path$\"")); // $"$path$": 0
	printf("%d\n", contain_var("$\"abc$\"")); // $"abc$": 0
	printf("%d\n", contain_var("$\"abc$\"\'$var\'\"dba$\"")); // $"abc$"'$var'"dba$": 1
}