#include "philosophers.h"

/**
 * @brief Checks if a given philosopher has taken too much time to eat.
 */
bool has_starved(t_philo *philo)
{
	t_table			*table;
	uint64_t		difference;
	struct timeval	time;

	table = get_table();
	gettimeofday(&time, NULL);
	difference = get_timestamp(time, table) - get_timestamp(philo->last_meal, table);
	if (difference >= table->time_die)
	{
		pthread_mutex_lock(&(table->stdout_mtx));
		printf("[!] - Difference is %lu...\n", difference);
		pthread_mutex_unlock(&(table->stdout_mtx));
		return (true);
	}
	return (false);
}

/**
 * @brief A function that is called when creating the monitor thread.
 * Looks at all the philosophers to check if someone died or if everyone
 * eat enough. In both cases, it calls for the end of the simulation by
 * setting `run_simulation` to `false`.
 */
void	*monitor(void *input)
{
	t_table	*table;
	int		i;
	bool	all_eaten_enough;

	(void)input;
	table = get_table();
	i = 0;
	all_eaten_enough = true;
	while (i < table->philo_count)
	{
		pthread_mutex_lock(&(table->philos[i].mtx));
		pthread_mutex_lock(&(table->mtx));
		if (table->philos[i].times_eaten < table->min_times_eaten)
			all_eaten_enough = false;
		pthread_mutex_unlock(&(table->mtx));
		if (has_starved(&table->philos[i]) == true)
		{
			update_last_change(&table->philos[i]);
			pthread_mutex_lock(&(table->mtx));
			table->run_simulation = false;
			pthread_mutex_unlock(&(table->mtx));
			pthread_mutex_lock(&(table->stdout_mtx));
			printf("%s%lu - %d died%s\n", KRED,
				get_timestamp(table->philos[i].last_change, table),
				i + 1, KNRM);
			pthread_mutex_unlock(&(table->stdout_mtx));
			pthread_mutex_unlock(&(table->philos[i].mtx));
			break ;
		}
		pthread_mutex_unlock(&(table->philos[i].mtx));
		i++;
		if (i == table->philo_count)
		{
			if (all_eaten_enough == true)
			{
				pthread_mutex_lock(&(table->mtx));
				table->run_simulation = false;
				pthread_mutex_unlock(&(table->mtx));
				break ;
			}
			i = 0; // infinite loop
			all_eaten_enough = true;
		}
	}
	return (NULL);
}

int	launch_threads(void)
{
	t_table		*table;
	int			i;

	table = get_table();
	i = 0;
	gettimeofday(&table->start, NULL);
	philos_init();
	pthread_mutex_lock(&(table->mtx));
	pthread_create(&table->waiter, NULL, monitor, NULL);
	while (i < table->philo_count)
	{
		pthread_mutex_lock(&(table->stdout_mtx));
		printf("[!] - Launching thread %d...\n", i);
		pthread_mutex_unlock(&(table->stdout_mtx));
		if (pthread_create(&table->philos[i].thread, NULL, philo_run, NULL) != 0)
		{
			pthread_mutex_unlock(&(table->mtx));
			sfwrite_stderr("[!] - Failed to create a thread!\n");
			sfwrite_stderr("[!] - Attempting to wait for created threads...\n");
			while (i >= 0)
				pthread_join(table->philos[--i].thread, NULL);
			return (1);
		}
		usleep(1000);
		i++;
	}
	pthread_mutex_unlock(&(table->mtx));
	pthread_mutex_lock(&(table->stdout_mtx));
	printf("[!] - Successfully launched %d threads!\n", i);
	pthread_mutex_unlock(&(table->stdout_mtx));
	return (0);
}

int	wait_on_threads(void)
{
	t_table		*table;
	int			i;

	table = get_table();
	i = 0;
	while (i < table->philo_count)
	{
		if (pthread_join(table->philos[i].thread, NULL) != 0)
			sfwrite_stderr("[!] - Failed to join a thread!");
		i++;
	}
	if (pthread_join(table->waiter, NULL) != 0)
		sfwrite_stderr("[!] - Failed to join a thread!");
	return (0);
}