#include "philosophers.h"

int	launch_threads(void)
{
	t_table		*table;
	int			i;

	table = get_table();
	i = 0;
	pthread_mutex_lock(&(table->mtx));
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
	sfwrite_stdout("[!] - Waiting for threads to finish...\n");
	i = 0;
	while (i < table->philo_count)
	{
		if (pthread_join(table->philos[i].thread, NULL) != 0)
			sfwrite_stderr("[!] - Failed to join a thread!");
		i++;
	}
	return (0);
}