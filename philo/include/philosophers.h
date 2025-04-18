/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fireinside <aisling.fontaine@pm.me>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 19:17:19 by fireinside        #+#    #+#             */
/*   Updated: 2025/04/08 19:17:19 by fireinside       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdarg.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <inttypes.h>
# include <threads.h>
# include <pthread.h>
# include <sys/time.h>

// Enums
typedef enum e_state
{
	EAT,
	SLEEP,
	THINK
}	t_state;

// Typedefs
// For readability of which variables represent a point in time
typedef int	t_timestamp;

// The main struct of the program, overseeing the whole simulation.
typedef struct s_table
{
	pthread_mutex_t	stdout_mtx;
	pthread_mutex_t	stderr_mtx;
}	t_table;

// Represents a singular philosopher, or thinker.
typedef struct s_philo
{
	__thrd_t			t;
	t_state				state;
	struct timeval		*last_change;
}	t_philo;

// Represents a fork and its mutex.
typedef struct s_fork
{
	int8_t			status;
	pthread_mutex_t	mtx;
}	t_fork;

t_table	*get_table(void);

// Thinkers functions - thinkers.c
void	*philo_init(void *input);

// General utils - utils.c
size_t	ft_strlen(char *str);

// Mutex-locked utils - safe_utils.c
size_t	sfwrite_stdout(char *buf);
size_t	sfwrite_stderr(char *buf);

#endif // PHILOSOPHERS_H
