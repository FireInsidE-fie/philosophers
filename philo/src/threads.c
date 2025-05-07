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
	pthread_mutex_lock(&philo->mtx);
	difference = get_timestamp(time) - get_timestamp(philo->last_meal);
	pthread_mutex_unlock(&philo->mtx);
	if (difference > table->time_die)
	{
		pthread_mutex_lock(&(table->stdout_mtx));
		printf("[!] - Difference is %lu...\n", difference);
		pthread_mutex_unlock(&(table->stdout_mtx));
		return (true);
	}
	return (false);
}

void	*monitor(void *input)
{
	t_table	*table;
	int		i;

	(void)input;
	table = get_table();
	i = 0;
	while (i < table->philo_count)
	{
		pthread_mutex_lock(&(table->mtx));
		if (has_starved(&table->philos[i]) == true)
		{
			update_last_change(&table->philos[i]);
			table->run_simulation = false;
			pthread_mutex_unlock(&(table->mtx));
			pthread_mutex_lock(&(table->stdout_mtx));
			printf("%s%li:%li - %d died%s\n", KRED,
				table->philos[i].last_change.tv_sec,
				(long)table->philos[i].last_change.tv_usec / 1000,
				table->philos[i].id, KNRM);
			pthread_mutex_unlock(&(table->stdout_mtx));
			break ;
		}
		pthread_mutex_unlock(&(table->mtx));
		i++;
		if (i == table->philo_count)
			i = 0; // infinite loop
	}
	return (NULL);
}

int	launch_threads(void)
{
	t_table		*table;
	int			i;

	table = get_table();
	i = 0;
	pthread_mutex_lock(&(table->mtx));
	pthread_create(&table->waiter, NULL, monitor, NULL);
	while (i < table->philo_count)
	{
		pthread_mutex_lock(&(table->stdout_mtx));
		printf("[!] - Launching thread %d...\n", i);
		pthread_mutex_unlock(&(table->stdout_mtx));
		if (pthread_create(&table->philos[i].thread, NULL, philo_init, &i) != 0)
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
	// sfwrite_stderr("[!] - Waiting for threads to finish...\n");
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