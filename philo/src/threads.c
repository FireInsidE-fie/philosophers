#include "philosophers.h"

/**
 * @brief Checks if a given philosopher has taken too much time to eat.
 */
static bool	has_starved(t_philo *philo)
{
	t_table			*table;
	uint64_t		diff;
	struct timeval	time;

	table = get_table();
	gettimeofday(&time, NULL);
	pthread_mutex_lock(&philo->mtx);
	diff = get_timestamp(time, table) - get_timestamp(philo->last_meal, table);
	pthread_mutex_unlock(&philo->mtx);
	if (diff >= table->time_die)
	{
		pthread_mutex_lock(&(table->stdout_mtx));
		printf("[!] - Difference is %lu...\n", diff);
		pthread_mutex_unlock(&(table->stdout_mtx));
		update_last_change(philo);
		pthread_mutex_lock(&table->mtx);
		table->run_simulation = false;
		pthread_mutex_unlock(&table->mtx);
		pthread_mutex_lock(&table->stdout_mtx);
		printf("%s%lu - %d died%s\n", KRED,
			get_timestamp(philo->last_change, table), philo->id, KNRM);
		pthread_mutex_unlock(&table->stdout_mtx);
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

	table = get_table();
	i = 0;
	all_eaten_enough = true;
	while (i < table->philo_count)
	{
		pthread_mutex_lock(&table->philos[i].mtx);
		pthread_mutex_lock(&table->mtx);
		if (all_eaten_enough == true
			&& table->philos[i].times_eaten < table->min_times_eaten)
			all_eaten_enough = false;
		pthread_mutex_unlock(&table->mtx);
		if (has_starved(&table->philos[i]) == true)
			return (input);
		pthread_mutex_unlock(&table->philos[i].mtx);
		i++;
		if (i == table->philo_count)
		{
			if (all_eaten_enough == true)
			{
				pthread_mutex_lock(&table->mtx);
				table->run_simulation = false;
				return (pthread_mutex_unlock(&table->mtx), input);
			}
			i = 0; // infinite loop
			all_eaten_enough = true;
		}
	}
	return (input);
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
		if (pthread_create(&table->philos[i].thread,
			NULL, philo_run, &table->philos[i]) != 0)
		{
			pthread_mutex_unlock(&(table->mtx));
			while (i >= 0)
				pthread_join(table->philos[--i].thread, NULL);
			return (1);
		}
		usleep(1000);
		i++;
	}
	pthread_mutex_unlock(&(table->mtx));
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