/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_printf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: estettle <estettle@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:08:23 by estettle          #+#    #+#             */
/*   Updated: 2025/02/19 14:16:18 by estettle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/**
 * @brief Writes to stdout after locking it with a mutex.
 * 
 * @param buf The string to write to stdout.
 * @return The number of bytes written. -1 if an error occurred.
 */
int	swrite_stdout(char *buf)
{
	size_t	count;
	mtx_t	stdout_mtx;

	mtx_lock(&stdout_mtx);
	count = write(1, buf, ft_strlen(buf));
	mtx_unlock(&stdout_mtx);
	return (count);
}

/**
 * @brief Writes to stderr after locking it with a mutex.
 * 
 * @param buf The string to write to stderr.
 * @return The number of bytes written. -1 if an error occurred.
 */
int	swrite_stdout(char *buf)
{
	size_t	count;
	mtx_t	stderr_mtx;

	mtx_lock(&stderr_mtx);
	count = write(1, buf, ft_strlen(buf));
	mtx_unlock(&stderr_mtx);
	return (count);
}