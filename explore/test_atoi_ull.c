#include <stdio.h>
#include <limits.h>

int check_ll_limit(char *str, int sign)
{
	unsigned long long	num;

	num = 0;
	if (*str == '-' || *str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		num = num * 10 + (*str - '0') ;
		str++;
	}
	if (sign > 0 && num > LLONG_MAX)
	{
		printf("too much\n");
		return (1);
	}
	if (sign < 0 && num > (unsigned long long)LLONG_MAX + 1)
	{
		printf("too little\n");
		return (1);
	}
	return (0);
}

unsigned long long	ft_atoi_ull(char *str)
{
	unsigned long long	num;

	num = 0;
	if (*str == '-' || *str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		num = num * 10 + (*str - '0') ;
		str++;
	}
	return (num);
}

int	get_exit_status(char *str)
{
	long long	num;
	int	sign;

	sign = 1;
	if (*str == '-')
		sign = -1;
	num = (long long) ft_atoi_ull(str) * sign;
	// printf("num: %lld\n", num);
	num = num % 256;
	if (num < 0)
		num = 256 + num;
	// printf("num modulo: %lld\n", num);

	return ((int) num);
}

int main()
{
	// printf("%d\n", check_ll_limit("9223372036854775807", 1));
	// printf("%d\n", check_ll_limit("9223372036854775808", 1));

	// printf("%d\n", check_ll_limit("-9223372036854775808", -1));
	// printf("%d\n", check_ll_limit("-9223372036854775809", -1));

	printf("%d\n", get_exit_status("2")); // 2
	printf("%d\n", get_exit_status("-2")); // 254
	printf("%d\n", get_exit_status("9223372036854775807")); // 255
	printf("%d\n", get_exit_status("-9223372036854775808")); // 0

}