#include <stdio.h>
int main(int argc, char **argv)
{
	int i;
	
	i = 1;
	printf("argc - 1: %d\n", argc - 1);
	while (--argc)
		printf("%s\n", argv[i++]);
}