#include <stdlib.h>
#include <stdio.h>
# include <stdarg.h>

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

char	*strjoin_num_free(int num, ...)
{
	va_list	strs;
	char	*temp;
	char	*arg;
	
	va_start(strs, num);
	arg = va_arg(strs, char *);
	temp = ft_strdup(arg);
	if (!temp)
		return (NULL); // add error msg
	free(arg);
	while (--num)
	{
		arg = va_arg(strs, char *);
		temp = ft_strjoin_free(temp, arg);
		if (!temp)
			return (NULL);
	}
	va_end(strs);
	return (temp);
}

int main()
{
	char *str1 = malloc(5);
	char *str2 = malloc(5);
	char *str3 = malloc(5);

	for (int i = 0; i < 4; i++)
	{
		str1[i] = 'a';
	}
	for (int i = 0; i < 4; i++)
	{
		str2[i] = 'b';
	}
	for (int i = 0; i < 4; i++)
	{
		str3[i] = 'c';
	}
	str1[4] = '\0';
	str2[4] = '\0';
	str3[4] = '\0';

	printf("%s\n", strjoin_num_free(3, str1, str2, str3));
}