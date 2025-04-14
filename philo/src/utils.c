/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fireinside <aisling.fontaine@pm.me>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 00:20:51 by fireinside        #+#    #+#             */
/*   Updated: 2025/04/14 20:44:19 by fireinside       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

size_t	ft_strlen(char *str)
{
	size_t	count;

	while (*str)
	{
		count++;
		str++;
	}
	return (count);
}
