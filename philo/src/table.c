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

/**
 * @brief Parses the argv input and initializes the table variables.
 *
 * @note Initializes `min_times_eaten` to -1 to put the unsigned integer to
 * its maximum value. This effectively negates the min_times_eaten limit.
 */
int	init_table(char **argv)
{
	t_table	*table;

	table = get_table();
	if (!table)
		return (1);
	table->run_simulation = true;
	table->n_philos = ft_atoi(argv[1]);
	table->philos = ft_calloc(table->n_philos, sizeof(t_philo));
	if (!table->philos)
		return (free(table), 1);
	table->forks = ft_calloc(table->n_philos, sizeof(t_fork));
	if (!table->forks)
		return (free(table->philos), free(table), 1);
	table->time_die = ft_atoi(argv[2]);
	table->time_eat = ft_atoi(argv[3]);
	table->time_sleep = ft_atoi(argv[4]);
	table->min_times_eaten = -1;
	if (argv[5])
		table->min_times_eaten = ft_atoi(argv[5]);
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

	table = get_table();
	if (pthread_mutex_init(&table->mtx, NULL) != 0
		|| (pthread_mutex_init(&table->stdout_mtx, NULL) != 0)
		|| (pthread_mutex_init(&table->stderr_mtx, NULL) != 0))
		return (printf("%s[!] - Failed to init mutexes!%s\n", KRED, KNRM),
			destroy_mutexes(), -1);
	i = 0;
	while (i < table->n_philos)
	{
		if (pthread_mutex_init(&table->philos[i].mtx, NULL)
			|| (pthread_mutex_init(&table->forks[i].mtx, NULL))
			|| (pthread_mutex_init(&table->forks[i].last_eater_mtx, NULL)))
			return (printf("%s[!] - Failed to init mutexes!%s\n", KRED, KNRM),
				destroy_mutexes(), -1);
		i++;
	}
	return (0);
}

void	destroy_mutexes(void)
{
	int		i;
	t_table	*table;

	table = get_table();
	pthread_mutex_lock(&table->mtx);
	if (pthread_mutex_destroy(&table->stdout_mtx) != 0)
		printf("%s[!] - Failed to destroy mutexes (stdout)!%s\n", KRED, KNRM);
	if (pthread_mutex_destroy(&table->stderr_mtx) != 0)
		printf("%s[!] - Failed to destroy mutexes (stderr)!%s\n", KRED, KNRM);
	i = 0;
	while (i < table->n_philos)
	{
		if (pthread_mutex_destroy(&table->philos[i].mtx)
			|| (pthread_mutex_destroy(&table->forks[i].mtx))
			|| (pthread_mutex_destroy(&table->forks[i].last_eater_mtx)))
			printf("%s[!] - Failed to destroy mutexes!%s\n", KRED, KNRM);
		i++;
	}
	pthread_mutex_unlock(&table->mtx);
	if (pthread_mutex_destroy(&table->mtx) != 0)
		printf("%s[!] - Failed to destroy mutexes (table)!%s\n", KRED, KNRM);
}
