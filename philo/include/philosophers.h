#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# define _DEFAULT_SOURCE

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <inttypes.h>
# include <pthread.h>
# ifndef __APPLE__
#  include <threads.h>
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
	THINK,
	SLEEP
}	t_action;

// Typedefs ====================================================================

// Represents a fork and its mutex.
typedef struct s_fork
{
	struct s_philo	*last_eater;
	pthread_mutex_t	last_eater_mtx;	// Mutex for checking and writing last_eater
	pthread_mutex_t	mtx;			// Mutex for eating with the fork
}	t_fork;

// Represents a singular philosopher or thinker.
typedef struct s_philo
{
	uint8_t			id;
	pthread_t		thread;
	t_fork			*left_fork;
	t_fork			*right_fork;
	t_action		action;			// The action the philo is currently doing
	struct timeval	last_change;	// The last time the philo changed states
	struct timeval	last_meal;		// The time the last meal started at
	uint32_t		times_eaten;
	pthread_mutex_t	mtx;
}	t_philo;

// The main struct of the program, overseeing the whole simulation.
typedef struct s_table
{
	uint8_t			n_philos;
	bool			run_simulation;		// Determines if the sim continues
	struct timeval	start;				// The time at which the sim started
	pthread_t		waiter;				// Monitoring thread
	uint32_t		time_die;			// Time it takes to die after eating
	uint32_t		time_eat;			// Time it takes to eat
	uint32_t		time_sleep;			// Time it takes to sleep
	uint32_t		min_times_eaten;	// Number of times everyone needs to eat
	t_philo			*philos;
	t_fork			*forks;
	pthread_mutex_t	stdout_mtx;			// Mutex for standard output
	pthread_mutex_t	stderr_mtx;			// Mutex for standard error
	pthread_mutex_t	mtx;				// Mutex for the table
}	t_table;

// Functions ===================================================================

// Table functions - table.c
t_table		*get_table(void);
int			init_table(char **argv);
void		clean_table(void);
int			init_mutexes(void);
void		destroy_mutexes(void);

// Thinkers functions - thinkers.c
void		*philo_run(void *input);
void		philos_init(void);

// Thinkers utils functions - thinkers_utils.c
void		update_last_change(t_philo *self);
int			pickup_forks_even(t_philo *self);
int			pickup_forks_uneven(t_philo *self);
void		drop_forks(const t_philo *self);

// Threads functions - threads.c
int			launch_threads(void);
int			wait_on_threads(void);

// General utils - utils.c
int			ft_atoi(const char *str);
size_t		ft_strlen(const char *str);
uint64_t	get_timestamp(const struct timeval time, const t_table *table);
void		ft_memset(void *mem, size_t size);
void		*ft_calloc(size_t number, size_t size);

// Mutex-locked utils - safe_utils.c
size_t		sfwrite_stdout(char *buf);
size_t		sfwrite_stderr(char *buf);

#endif // PHILOSOPHERS_H
