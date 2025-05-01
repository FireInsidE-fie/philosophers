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

static int	philo_init_mutexes(t_philo *self)
{
	if (pthread_mutex_init(&self->mtx, NULL) == -1)
		return (write(1, "[!] - Failed to init a philo's mutex!\n", 38), -1);
	return (0);
}

void	*philo_init(void *input)
{
	t_philo self;

	philo_init_mutexes(&self);
	pthread_mutex_lock(&self.mtx);
	self.id = *(int *)input;
	pthread_mutex_lock(&(get_table()->stdout_mtx));
	printf("[!] - Creating philosopher %d...\n", self.id);
	pthread_mutex_unlock(&(get_table()->stdout_mtx));
	if (gettimeofday(&self.last_change, NULL) == -1)
		sfwrite_stderr("[!] - Failed to get time of day!\n");
	pthread_mutex_lock(&(get_table()->stdout_mtx));
	printf("%li - %d isn't thinking...\n", self.last_change.tv_usec / 1000, self.id);
	pthread_mutex_unlock(&(get_table()->stdout_mtx));
	pthread_mutex_unlock(&self.mtx);
	// start philo main loop
	philo_destroy(self); // put this at the end of the main loop
	return (NULL);
}
