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
	free(self.last_change);
	sfwrite_stdout("[!] - Destroying philosopher...\n");
}

void	*philo_init(void *input)
{
	t_philo	self;

	(void)input;
	sfwrite_stdout("[!] - Creating philosopher...\n");
	self.last_change = malloc(sizeof(struct timeval));
	if (!self.last_change)
		return (NULL);
	gettimeofday(self.last_change, NULL);
	pthread_mutex_lock(&(get_table()->stdout_mtx));
	printf("%li\n", self.last_change->tv_sec);
	pthread_mutex_unlock(&(get_table()->stdout_mtx));
	// start philo main loop
	philo_destroy(self); // put this at the end of the main loop
	return (NULL);
}
