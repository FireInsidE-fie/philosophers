#include "philosophers.h"

/**
 * @brief Checks if a given philosopher has taken too much time to eat.
 */
static bool	has_starved(t_table *table, t_philo *philo)
{
	uint64_t		diff;
	struct timeval	time;

	table = get_table();
	pthread_mutex_lock(&philo->mtx);
	gettimeofday(&time, NULL);
	diff = get_timestamp(time, table) - get_timestamp(philo->last_meal, table);
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
		return (pthread_mutex_unlock(&philo->mtx), true);
	}
	return (pthread_mutex_unlock(&philo->mtx), false);
}

/**
 * Checks if a philosopher has eaten enough times.
 */
static bool	eaten_enough(t_table *table, t_philo *philo)
{
	bool	ret;

	pthread_mutex_lock(&philo->mtx);
	pthread_mutex_lock(&table->mtx);
	ret = true;
	if (philo->times_eaten < table->min_times_eaten)
		ret = false;
	pthread_mutex_unlock(&table->mtx);
	pthread_mutex_unlock(&philo->mtx);
	return (ret);
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
	bool	all_satisfied;

	table = get_table();
	i = 0;
	all_satisfied = true;
	while (i < table->n_philos)
	{
		all_satisfied = all_satisfied & eaten_enough(table, &table->philos[i]);
		if (has_starved(table, &table->philos[i]) == true)
			return (input);
		if (++i == table->n_philos)
		{
			if (all_satisfied == true)
			{
				pthread_mutex_lock(&table->mtx);
				table->run_simulation = false;
				return (pthread_mutex_unlock(&table->mtx), input);
			}
			i = 0; // infinite loop
			all_satisfied = true;
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
	pthread_create(&table->monitor, NULL, monitor, NULL);
	while (i < table->n_philos)
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
	while (i < table->n_philos)
	{
		if (pthread_join(table->philos[i].thread, NULL) != 0)
			sfwrite_stderr("[!] - Failed to join a thread!");
		i++;
	}
	if (pthread_join(table->monitor, NULL) != 0)
		sfwrite_stderr("[!] - Failed to join a thread!");
	return (0);
}
