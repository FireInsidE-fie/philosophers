/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fireinside <aisling.fontaine@pm.me>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 19:16:56 by fireinside        #+#    #+#             */
/*   Updated: 2025/04/08 19:16:56 by fireinside       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static int	init_mutexes(void)
{
	t_table	*table;

	printf("[!] - Initializing mutexes...\n");
	table = get_table();
	if (pthread_mutex_init(&table->stdout_mtx, NULL) != 0)
		return (write(1, "[!] - Failed to init mutexes!\n", 30), -1);
	if (pthread_mutex_init(&table->stderr_mtx, NULL) != 0)
		return (write(1, "[!] - Failed to init mutexes!\n", 30), -1);
	printf("[!] - Successfully initialized mutexes!\n");
	return (0);
}

static int	destroy_mutexes(void)
{
	t_table	*table;

	printf("[!] - Destroying mutexes...\n");
	table = get_table();
	pthread_mutex_lock(&table->table_mtx);
	if (pthread_mutex_destroy(&table->stdout_mtx) != 0)
		return (write(1, "[!] - Failed to destroy mutexes!\n", 33), -1);
	if (pthread_mutex_destroy(&table->stderr_mtx) != 0)
		return (write(1, "[!] - Failed to destroy mutexes!\n", 33), -1);
	pthread_mutex_unlock(&table->table_mtx);
	if (pthread_mutex_destroy(&table->table_mtx) != 0)
		return (write(1, "[!] - Failed to destroy mutexes!\n", 33), -1);
	printf("[!] - Successfully destroyed mutexes!\n");
	return (0);
}

int	main(int argc, char **argv)
{
	t_table	*table;

	if (argc < 5 || argc > 6)
		return (1);
	table = get_table();
	if (!table)
		return (1);
	table->philo_count = ft_atoi(argv[1]);
	table->philos = malloc(sizeof(pthread_t) * table->philo_count);
	if (!table->philos)
		return (free(table), 1);
	printf("[!] - Philo count = %d\n", table->philo_count);
	init_mutexes();
	launch_threads();
	wait_on_threads();
	destroy_mutexes();
	free(table->philos);
	free(table);
	return (0);
}
