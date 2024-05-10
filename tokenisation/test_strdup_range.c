#include <stdio.h>
#include <stdlib.h>

char *strdup_range(char *start, char *end)
{
	char *ptr;
	size_t	n;
	int i;

	n = end - start;
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
	ptr[i+1] = '\0';
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
int main()
{
	char text[4];
	text[0] = 'a';
	text[1] = 'b';
	text[2] = 'c';
	text[3] = '\0';

	char *start;
	char *end;
	start = text;
	end = start + 1;

	printf("%c, %c\n", *start, *end);
	printf("%ld\n", end - start);
	printf("%s\n", strdup_range(start, end));
	printf("%d\n", ft_strlen(strdup_range(start, end)));
	if (strdup_range(start, end)[2] == '\0')
		printf("nullterm\n");
}