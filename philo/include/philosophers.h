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

# define _DEFAULT_SOURCE

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <inttypes.h>
# ifdef __APPLE__
#  include <pthread.h>
# else
#  include <thread.h>
# endif
# include <sys/time.h>
# include <stdint.h>
# include <stdbool.h>

// Color macros ================================================================
# define KNRM  "\x1B[0m"
# define KRED  "\x1B[31m"
# define KGRN  "\x1B[32m"
# define KYEL  "\x1B[33m"
# define KBLU  "\x1B[34m"
# define KMAG  "\x1B[35m"
# define KCYN  "\x1B[36m"
# define KWHT  "\x1B[37m"

// Enums =======================================================================
typedef enum e_action
{
	EAT,
	SLEEP,
	THINK
}	t_action;

// Typedefs ====================================================================

// Represents a fork and its mutex.
typedef struct s_fork
{
	int8_t			status;
	struct s_philo	*last_eater;
	pthread_mutex_t	mtx;
}	t_fork;

// Represents a singular philosopher or thinker.
typedef struct s_philo
{
	uint8_t			id;
	bool			alive;
	pthread_t		thread;
	t_fork			*left_fork;
	t_fork			*right_fork;
	t_action		action;			// The action the philo is currently doing
	struct timeval	last_change;	// The last time the philo changed actions
	uint32_t		times_eaten;
	pthread_mutex_t	mtx;			// TODO : is this even useful?
}	t_philo;

// The main struct of the program, overseeing the whole simulation.
typedef struct s_table
{
	uint8_t			philo_count;
	bool			run_simulation;		// Determines if the sim continues
	pthread_t		waiter;				// Monitoring thread
	pthread_mutex_t	mtx;				// Mutex for the table
	pthread_mutex_t	stdout_mtx;			// Mutex for standard output
	pthread_mutex_t	stderr_mtx;			// Mutex for standard error
	uint32_t		time_die;			// Time a philo takes to die after eating
	uint32_t		time_eat;			// Time it takes to eat
	uint32_t		time_sleep;			// Time it takes to sleep
	uint32_t		min_times_eaten;	// Number of times all philos need to eat
	t_philo			*philos;
	t_fork			*forks;
}	t_table;

// Functions ===================================================================

// Table functions - table.c
t_table		*get_table(void);
int			init_table(char **argv);
void		clean_table(void);
int			init_mutexes(void);
void		destroy_mutexes(void);

// Thinkers functions - thinkers.c
void		*philo_init(void *input);

// Threads functions - threads.c
int			launch_threads(void);
int			wait_on_threads(void);

// General utils - utils.c
int			ft_atoi(const char *str);
size_t		ft_strlen(const char *str);
uint64_t	get_timestamp(struct timeval timeval);

// Mutex-locked utils - safe_utils.c
size_t		sfwrite_stdout(char *buf);
size_t		sfwrite_stderr(char *buf);

#endif // PHILOSOPHERS_H
