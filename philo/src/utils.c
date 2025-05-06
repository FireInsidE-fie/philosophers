/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fireinside <aisling.fontaine@pm.me>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 00:20:51 by fireinside        #+#    #+#             */
/*   Updated: 2025/04/29 15:40:16 by fireinside       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/**
 * @brief Convers a string into its integer representation.
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

/**
 * @brief Returns the current time in milliseconds.
 */
uint64_t	get_timestamp(struct timeval timeval)
{
	uint64_t	timestamp;

	timestamp = timeval.tv_sec * 1000 + timeval.tv_usec / 1000;
	return (timestamp);
}
