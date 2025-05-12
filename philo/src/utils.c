#include "philosophers.h"

/**
 * @brief Converts a string into its integer representation.
 * This function accepts any number of white space (isspace(3)) as a suffix,
 * as well as one minus (-) or plus (-) sign.
 *
 * @param str The string to convert.
 * @return The converted string.
 */
int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	converted;

	i = 0;
	sign = 1;
	converted = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			sign = -sign;
	while (str[i] >= '0' && str[i] <= '9')
	{
		converted += str[i++] - '0';
		if (str[i] >= '0' && str[i] <= '9')
			converted *= 10;
	}
	return (converted * sign);
}

size_t	ft_strlen(const char *str)
{
	size_t	count;

	count = 0;
	while (*str)
	{
		count++;
		str++;
	}
	return (count);
}

void	ft_memset(void *mem, size_t size)
{
	while (size--)
		*(char *)(mem++) = 0;
}

void	*ft_calloc(size_t number, size_t size)
{
	size_t	full_size;
	void	*new_mem;

	full_size = number * size;
	new_mem = malloc(full_size);
	if (!new_mem)
		return (NULL);
	ft_memset(new_mem, full_size);
	return (new_mem);
}

/**
 * @brief Returns the current time in milliseconds since the simulation
 * started.
 */
uint64_t	get_timestamp(const struct timeval time, const t_table* table)
{
	uint64_t	timestamp;

	timestamp = time.tv_sec * 1000 + time.tv_usec / 1000
		- (table->start.tv_sec * 1000 + table->start.tv_usec / 1000);
	return (timestamp);
}
