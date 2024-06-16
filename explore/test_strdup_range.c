#include <stdio.h>
#include <stdlib.h>

char *strdup_range(char *start, char *end)
{
	char *ptr;
	size_t	n;
	int i;

	n = end - start + 1;
	ptr = malloc(sizeof(char) * (n + 1));
	printf("n + 1: %ld\n", n + 1);
	if (!ptr)
		return (0);
	i = 0;
	while (start != end)
	{
		ptr[i] = *start;
		start++;
		printf("%c\n", ptr[i]);
		i++;
	}
	ptr[i] = *end;
	printf("%c\n", ptr[i]);
	ptr[i+1] = '\0';
	printf("i+1: %d\n", i+1);
	return (ptr);
}

int ft_strlen(char *str)
{
	int i = 0;
	while (str[i])
	{
		i++;
	}
	return (i);
}

char	*ft_strdup(char *src)
{
	char		*ptr;
	size_t		n;
	int			i;

	n = ft_strlen(src);
	printf("n: %zu\n", n);
	ptr = (char *)malloc(sizeof(char) * (n + 1));
	if (!ptr)
		return (0);
	i = -1;
	while (src[++i] != '\0')
		ptr[i] = src[i];
	ptr[i] = '\0';
	return (ptr);
}

int main()
{
	char text[7];
	text[0] = 'a';
	text[1] = 'b';
	text[2] = 'f';
	text[3] = 'd';
	text[4] = 'e';
	text[5] = '=';
	text[6] = '\0';

	char *start;
	char *end;
	start = text;
	end = start + 4;

	printf("%c, %c\n", *start, *end);
	printf("%ld\n", end - start);
	printf("%s\n", strdup_range(start, end));
	// printf("%d\n", ft_strlen(strdup_range(start, end)));
	// if (strdup_range(start, end)[2] == '\0')
	// 	printf("nullterm\n");
}


// int main()
// {
// 	printf("%d\n", ft_strlen(ft_strdup("")));
// 	if (ft_strdup("")[0] == '\0')
// 		printf("empty\n");
// }