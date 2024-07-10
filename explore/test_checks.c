#include <stdio.h>
#include <unistd.h>

int	check_quotes(char *line)
{
	int		i;
	char	quote_t;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '"' || line[i] == 39)
		{
			quote_t = line[i];
			i++;
			while (line[i] != '\0' && line[i] != quote_t)
				i++;
			if (line[i] == '\0')
				return (0);
			else
				i++;
		}
		else
			i++;
	}
	return (1);
}

int	pipe_mid(int i, char *line)
{
	i++;
	while (line[i] == 32 || line[i] == 9)
		i++;
	if (line[i] == '\0')
		return (write(1, ">\n", 2), 0);
	return (1);
}

int	check_pipes(char *line)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (line[i] != '\0')
	{
		if (line[0] == '|' && line [1] != '|')
			return (write(2, "minishell: syntax error near unexpected token `|'\n", 46), 0);
		j = i;
		while ((line[i] == 32 || line[i] == 9))
		{
			i++;
			if (line[i] == '|' && line [i + 1] != '|' && j == 0)
				return (write(2, "minishell: syntax error near unexpected token `|'\n", 46), 0);
		}
		if (line[i] == '|' && line [i + 1] != '|' && line[i - 1] != '|')
		{
			if (pipe_mid(i, line) == 0)
				return (0);
		}
		i++;
	}
	return (1);
}

int	check_direct(char *line)
{
	int	i;

	i = 0;
	while (line[i] != '\0')
	{
		if ((line[i] == '>' && line[i + 1] == '<') || (line[i] == '<' && line[i + 1] == '>'))
			return (0);
			// return (("minishell: syntax error near unexpected token\n", 2), 0);
		else if (((line[i] == '>' && line[i + 1] == '>') || (line[i] == '<' && line[i + 1] == '<')))
		{
			i = i + 2;
			while (line [i] == ' ' || line[i] == '\t')
				i++;
			if (line[i] == '\0')
				return (0);
				// return (ft_putstr_fd("minishell: syntax error near unexpected token", 2), 0);
		}
		else if ((line[i] == '>' || line[i] == '<' ))
		{
			i++;
			while (line [i] == ' ' || line[i] == '\t')
				i++;
			if (line[i] == '\0')
				return (0);
		}
		i++;
	}
	return (1);
}

int main()
{
	// printf("%d\n", check_quotes("abc\"\'\"def\n"));
	// printf("%d\n", check_pipes("abc | "));
	printf("%d\n", check_direct("\"><\""));
	
}