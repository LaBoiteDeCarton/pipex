#include "libft.h"

int	ft_atoi(const char *str)
{
	int	res;
	int	signe;

	res = 0;
	signe = 1;
	while (*str == ' ' || *str == '\r' || *str == '\n'
		|| *str == '\f' || *str == '\v' || *str == '\t')
		str++;
	if (*str == '-')
		signe = -1;
	if (*str == '-' || *str == '+')
		str++;
	while (ft_isdigit(*str))
	{
		res = res * 10 + signe * (*str - '0');
		str++;
	}
	return (res);
}