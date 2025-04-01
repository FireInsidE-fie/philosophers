/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: estettle <estettle@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:11:21 by estettle          #+#    #+#             */
/*   Updated: 2025/02/18 11:11:57 by estettle         ###   ########.fr       */
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

	table = get_table();
	if (pthread_mutex_init(&table->stdout_mtx, NULL) != 0)
		return (write(1, "Failed to init mutexes!\n", 25));
	if (pthread_mutex_init(&table->stderr_mtx, NULL) != 0)
		return (write(1, "Failed to init mutexes!\n", 25));
	return (0);
}

static int	destroy_mutexes(void)
{
	t_table	*table;

	table = get_table();
	if (pthread_mutex_destroy(&table->stdout_mtx) != 0)
		return (write(1, "Failed to destroy mutexes!\n", 25));
	if (pthread_mutex_init(&table->stderr_mtx, NULL) != 0)
		return (write(1, "Failed to destroy mutexes!\n", 25));
	return (0);
}

int	main(void)
{
	memset(get_table(), 0, sizeof(t_table));
	init_mutexes();
	destroy_mutexes();
	return (0);
}
