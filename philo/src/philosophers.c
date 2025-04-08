/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fireinside <firefoxSpinnie@protonmail.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 19:16:56 by fireinside        #+#    #+#             */
/*   Updated: 2025/04/08 19:16:56 by fireinside       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_table	*get_table(void)
{
	static t_table	table;

	return (&table);
}

static int	init_mutexes(void)
{
	t_table	*table;

	printf("[!] - Initializing mutexes...\n");
	table = get_table();
	if (pthread_mutex_init(&table->stdout_mtx, NULL) != 0)
		return (write(1, "Failed to init mutexes!\n", 25));
	if (pthread_mutex_init(&table->stderr_mtx, NULL) != 0)
		return (write(1, "Failed to init mutexes!\n", 25));
	printf("[!] - Successfully initalized mutexes!\n");
	return (0);
}

static int	destroy_mutexes(void)
{
	t_table	*table;

	printf("[!] - Destroying mutexes...\n");
	table = get_table();
	if (pthread_mutex_destroy(&table->stdout_mtx) != 0)
		return (write(1, "Failed to destroy mutexes!\n", 25));
	if (pthread_mutex_init(&table->stderr_mtx, NULL) != 0)
		return (write(1, "Failed to destroy mutexes!\n", 25));
	printf("[!] - Successfully destroyed mutexes!\n");
	return (0);
}

int	main(void)
{
	memset(get_table(), 0, sizeof(t_table));
	init_mutexes();
	destroy_mutexes();
	return (0);
}
