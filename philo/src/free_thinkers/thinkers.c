/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thinkers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fireinside <aisling.fontaine@pm.me>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 19:17:25 by fireinside        #+#    #+#             */
/*   Updated: 2025/04/29 15:56:02 by fireinside       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	philo_destroy(t_philo self)
{
	pthread_mutex_lock(&(get_table()->stdout_mtx));
	printf("[!] - Destroying philosopher %d...\n", self.id);
	pthread_mutex_unlock(&(get_table()->stdout_mtx));
}

void	*philo_init(void *input)
{
	t_table	*table;
	t_philo self;

	self.id = *(int *)input;
	table = get_table();
	pthread_mutex_lock(&(table->stdout_mtx));
	printf("[!] - Creating philosopher %d...\n", self.id);
	pthread_mutex_unlock(&(table->stdout_mtx));
	if (gettimeofday(&self.last_change, NULL) == -1)
		sfwrite_stderr("[!] - Failed to get time of day!\n");
	pthread_mutex_lock(&(table->stdout_mtx));
	printf("%li - %d isn't thinking...\n", self.last_change.tv_usec / 1000, self.id);
	pthread_mutex_unlock(&(table->stdout_mtx));
	// start philo main loop
	philo_destroy(self); // put this at the end of the main loop
	pthread_exit(NULL);
}
