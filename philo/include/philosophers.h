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
# include <stdint.h>

// Enums =======================================================================
typedef enum e_state
{
	EAT,
	SLEEP,
	THINK
}	t_state;

// Typedefs ====================================================================

// Represents a fork and its mutex.
typedef struct s_fork
{
	int8_t			status;
	pthread_mutex_t	mtx;
}	t_fork;

// Represents a singular philosopher or thinker.
typedef struct s_philo
{
	int				id;
	t_state			state;
	struct timeval	last_change;
	pthread_mutex_t	mtx;
}	t_philo;

// The main struct of the program, overseeing the whole simulation.
typedef struct s_table
{
	int				philo_count;
	pthread_mutex_t	table_mtx;
	pthread_mutex_t	stdout_mtx;
	pthread_mutex_t	stderr_mtx;
	pthread_t		*philos;
}	t_table;

// Functions ===================================================================

// Main file - philosophers.c
t_table	*get_table(void);

// Thinkers functions - thinkers.c
void	*philo_init(void *input);

// Threads functions - threads.c
int		launch_threads(void);
int		wait_on_threads(void);

// General utils - utils.c
int		ft_atoi(const char *str);
size_t	ft_strlen(const char *str);

// Mutex-locked utils - safe_utils.c
size_t	sfwrite_stdout(char *buf);
size_t	sfwrite_stderr(char *buf);

#endif // PHILOSOPHERS_H
