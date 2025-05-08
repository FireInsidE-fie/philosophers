#include "philosophers.h"

/**
 * @brief Writes to stdout after locking it with a mutex.
 * 
 * @param buf The string to write to stdout.
 * @return The number of bytes written. -1 if an error occurred.
 */
size_t	sfwrite_stdout(char *buf)
{
	size_t	count;

	count = ft_strlen(buf);
	pthread_mutex_lock(&(get_table()->stdout_mtx));
	count = write(1, buf, count);
	pthread_mutex_unlock(&(get_table()->stdout_mtx));
	return (count);
}

/**
 * @brief Writes to stderr after locking it with a mutex.
 * 
 * @param buf The string to write to stderr.
 * @return The number of bytes written. -1 if an error occurred.
 */
size_t	sfwrite_stderr(char *buf)
{
	size_t	count;

	count = ft_strlen(buf);
	pthread_mutex_lock(&(get_table()->stderr_mtx));
	count = write(2, buf, count);
	pthread_mutex_unlock(&(get_table()->stderr_mtx));
	return (count);
}
