#include <stdlib.h>
#include <stdio.h>
# include <stdarg.h>
#include <unistd.h>


void	free_num(int num, ...)
{
	va_list	args;

	va_start(args, num);
	while (num--)
	{
		free(va_arg(args, void *));
	}
	va_end(args);
}

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

char	*strdup_range(char *start, char *end)
{
	char	*ptr;
	size_t	n;
	int		i;

	n = end - start + 1;
	ptr = malloc(sizeof(char) * (n + 1));
	if (!ptr)
		return (0);
	i = 0;
	while (start != end)
	{
		ptr[i] = *start;
		start++;
		i++;
	}
	ptr[i] = *end;
	ptr[i + 1] = '\0';
	return (ptr);
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*ptr;
	size_t	tot_len;
	int		i;
	int		j;
	
	if (!s1 || !s2)
		return (free_num(2, s1, s2), NULL);
	// if (!s1) 
	// 	return (s2);
	// if (!s2)
	// 	return (0);
	tot_len = ft_strlen(s1) + ft_strlen(s2) + 1;
	ptr = (char *)malloc(sizeof(char) * tot_len);
	if (!ptr)
		return (0);
	i = 0;
	j = 0;
	while (s1[j])
		ptr[i++] = s1[j++];
	j = 0;
	while (s2[j])
		ptr[i++] = s2[j++];
	ptr[i] = '\0';
	free_num(2, s1, s2);
	// free(s1);
	// free(s2);
	return (ptr);
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

char	*strjoin_num_free(int num, ...)
{
	va_list	strs;
	char	*temp;
	char	*arg;
	
	va_start(strs, num);
	arg = va_arg(strs, char *);
	temp = ft_strdup(arg);
	if (!temp)
		return (ft_putstr_fd("Malloc error creating strjoin temp\n", 2), NULL); // add error msg
	free(arg);
	while (--num)
	{
		arg = va_arg(strs, char *);
		temp = ft_strjoin_free(temp, arg);
		if (!temp)
			return (ft_putstr_fd("Malloc error creating strjoin temp\n", 2), NULL);
	}
	va_end(strs);
	return (temp);
}


char	*remove_quote_str(char *str)
{
	char	quote;
	int		i;
	int		start;
	char	*before_quote;

	quote = 0;
	i = 0;
	start = 0;
	while (str[i])
	{
		if (str[i] == 34 || str[i] == 39)
		{
			if (quote == 0)
			{
				quote = str[i];
				if (i == 0)
					before_quote = ft_strdup("");
				else
					before_quote = strdup_range(&str[0], &str[i - 1]);
				// if (before_quote[0] == '\0')
				// 	printf("before_quote: empty\n");
				// else
				// 	printf("before_quote: %s\n", before_quote);
				start = i + 1;
			}
			else if (quote == str[i])
			{
				str = remove_quote_join(str, before_quote, &start, &i);
				quote = 0;
			}
		}
		i++;
	}
	return (str);
}

int main()
{
	char *str;
	// str = ft_strdup("abc\"$var\"");
	// str = ft_strdup("abc\"$var\"def\"ghi\"");
	// str = ft_strdup("z\' abc \'z\' abc \'"); // z abc z abc
	// str = ft_strdup("\"dddd\"a\'ssss\'"); // ddddassss
	// str = ft_strdup("\"dddd\"\'ssss\'"); // ddddssss
	// str = ft_strdup("a\"dddd\"\'ssss\'"); // addddssss
	str = ft_strdup("abc\"$var\'extra\"def\"ghi\"\'abc\'\'\"\'"); // abc$var'extradefghiabc"

	printf("before: %s\n", str);
	printf("after: %s\n", remove_quote_str(str));
}