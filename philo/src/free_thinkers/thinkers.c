/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thinkers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fireinside <aisling.fontaine@pm.me>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 19:17:25 by fireinside        #+#    #+#             */
/*   Updated: 2025/04/14 20:38:52 by fireinside       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	philo_destroy(t_philo self)
{
	pthread_mutex_lock(&(get_table()->stdout_mtx));
	printf("[!] - Destroying philosopher %d...\n", self.id);
	pthread_mutex_unlock(&(get_table()->stdout_mtx));
}

void	*philo_init(void *input)
{
	t_philo	self;

	(void)input;
	sfwrite_stdout("[!] - Creating philosopher...\n");
	if (gettimeofday(&self.last_change, NULL) == -1)
		sfwrite_stderr("[!] - Failed to get time of day!\n");
	pthread_mutex_lock(&(get_table()->stdout_mtx));
	printf("%li\n", self.last_change.tv_usec / 1000);
	pthread_mutex_unlock(&(get_table()->stdout_mtx));
	// start philo main loop
	philo_destroy(self); // put this at the end of the main loop
	return (NULL);
}
