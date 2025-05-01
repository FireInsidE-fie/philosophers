#include "philosophers.h"

int	launch_threads(void)
{
	t_table		*table;
	int			i;

	table = get_table();
	i = 0;
	while (i < table->philo_count)
	{
		pthread_mutex_lock(&(get_table()->stdout_mtx));
		printf("[!] - Launching thread %d...\n", i);
		pthread_mutex_unlock(&(get_table()->stdout_mtx));
		if (pthread_create(&table->philos[i], NULL, philo_init, &i) != 0)
		{
			sfwrite_stderr("[!] - Failed to create a thread!\n");
			sfwrite_stderr("[!] - Attempting to wait for create threads...\n");
			while (i >= 0)
				pthread_join(table->philos[--i], NULL);
			return (1);
		}
		i++;
	}
	return (0);
}

int	wait_on_threads(void)
{
	t_table		*table;
	int			i;

	table = get_table();
	i = table->philo_count;
	while (i < table->philo_count)
	{
		if (pthread_join(table->philos[--i], NULL) != 0)
			sfwrite_stderr("[!] - Failed to join a thread!");
	}
	return (0);
}