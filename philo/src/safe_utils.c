/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fireinside <firefoxSpinnie@protonmail.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 19:16:31 by fireinside        #+#    #+#             */
/*   Updated: 2025/04/08 19:16:31 by fireinside       ###   ########.fr       */
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
	size_t					count;

	pthread_mutex_lock(&(get_table()->stdout_mtx));
	count = write(1, buf, ft_strlen(buf));
	pthread_mutex_unlock(&(get_table()->stdout_mtx));
	return (count);
}

/**
 * @brief Writes to stderr after locking it with a mutex.
 * 
 * @param buf The string to write to stderr.
 * @return The number of bytes written. -1 if an error occurred.
 */
int	swrite_stderr(char *buf)
{
	size_t					count;

	pthread_mutex_lock(&(get_table()->stderr_mtx));
	count = write(2, buf, ft_strlen(buf));
	pthread_mutex_unlock(&(get_table()->stderr_mtx));
	return (count);
}
