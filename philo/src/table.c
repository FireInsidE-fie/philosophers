#include "philosophers.h"

/**
 * @brief Initializes the philosophers' table if needed and returns a pointer
 * to it.
 */
t_table	*get_table(void)
{
	static t_table	*table;

	if (!table)
	{
		table = malloc(sizeof(t_table));
		if (!table)
			return (NULL);
		memset(table, 0, sizeof(t_table));
	}
	return (table);
}

int	init_table(char **argv)
{
	t_table	*table;

	table = get_table();
	if (!table)
		return (1);
	table->philo_count = ft_atoi(argv[1]);
	table->philos = malloc(sizeof(t_philo) * table->philo_count); // Probably add 1 here to have a monitoring thread
	if (!table->philos)
		return (free(table), 1);
	table->forks = malloc(sizeof(t_fork) * table->philo_count);
	if (!table->forks)
		return (free(table->philos), free(table), 1);
	return (0);
}

void	clean_table(void)
{
	t_table	*table;

	table = get_table();
	destroy_mutexes();
	free(table->philos);
	free(table->forks);
	free(table);
}

int	init_mutexes(void)
{
	int		i;
	t_table	*table;

	printf("[!] - Initializing mutexes...\n");
	table = get_table();
	if (pthread_mutex_init(&table->mtx, NULL) != 0
		|| (pthread_mutex_init(&table->stdout_mtx, NULL) != 0)
		|| (pthread_mutex_init(&table->stderr_mtx, NULL) != 0))
		return (write(1, "[!] - Failed to init mutexes!\n", 30),
			destroy_mutexes(), -1);
	i = 0;
	while (i < table->philo_count)
	{
		if (pthread_mutex_init(&table->philos[i].mtx, NULL)
			|| (pthread_mutex_init(&table->forks[i].mtx, NULL)))
			return (write(1, "[!] - Failed to init mutexes!\n", 30),
				destroy_mutexes(), -1);
		i++;
	}
	printf("[!] - Successfully initialized mutexes!\n");
	return (0);
}

void	destroy_mutexes(void)
{
	int		i;
	t_table	*table;

	printf("[!] - Destroying mutexes...\n");
	table = get_table();
	pthread_mutex_lock(&table->mtx);
	if (pthread_mutex_destroy(&table->stdout_mtx) != 0)
		write(1, "[!] - Failed to destroy mutexes!\n", 33);
	if (pthread_mutex_destroy(&table->stderr_mtx) != 0)
		write(1, "[!] - Failed to destroy mutexes!\n", 33);
	i = 0;
	while (i < table->philo_count)
	{
		if (pthread_mutex_destroy(&table->philos[i].mtx)
			|| (pthread_mutex_destroy(&table->forks[i].mtx)))
			write(1, "[!] - Failed to destroy mutexes!\n", 30);
		i++;
	}
	pthread_mutex_unlock(&table->mtx);
	if (pthread_mutex_destroy(&table->mtx) != 0)
		write(1, "[!] - Failed to destroy mutexes!\n", 33);
	printf("[!] - Finished destroying mutexes!\n");
}
