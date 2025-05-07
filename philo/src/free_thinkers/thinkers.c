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

void	update_last_change(t_philo *self)
{
	pthread_mutex_lock(&self->mtx);
	if (gettimeofday(&self->last_change, NULL) == -1)
		sfwrite_stderr("[!] - Failed to get time of day!\n");
	pthread_mutex_unlock(&self->mtx);
}

static void	philo_eat(t_table *table, t_philo *self)
{
	pthread_mutex_lock(&table->forks[self->id - 1].mtx);
	if (self->id == table->philo_count)
		pthread_mutex_lock(&table->forks[0].mtx);
	else
		pthread_mutex_lock(&table->forks[self->id].mtx);
	pthread_mutex_lock(&self->mtx);
	if (gettimeofday(&self->last_meal, NULL) == -1)
		sfwrite_stderr("[!] - Failed to get time of day!\n");
	pthread_mutex_unlock(&self->mtx);
	update_last_change(self);
	pthread_mutex_lock(&(table->stdout_mtx));
	printf("%s%li:%li - %d is eating...%s\n", KYEL,
		self->last_change.tv_sec, (long)self->last_change.tv_usec / 1000,
		self->id, KNRM);
	pthread_mutex_unlock(&(table->stdout_mtx));
	usleep(table->time_eat * 1000);
	pthread_mutex_unlock(&table->forks[self->id - 1].mtx);
	if (self->id == table->philo_count)
		pthread_mutex_unlock(&table->forks[0].mtx);
	else
		pthread_mutex_unlock(&table->forks[self->id].mtx);
}

/**
 * @brief Philosophers think while they are waiting for forks.
 */
static void	philo_think(t_table *table, t_philo *self)
{
	update_last_change(self);
	pthread_mutex_lock(&(table->stdout_mtx));
	printf("%s%li:%li - %d is thinking...%s\n", KGRN,
		self->last_change.tv_sec, (long)self->last_change.tv_usec / 1000,
		self->id, KNRM);
	pthread_mutex_unlock(&(table->stdout_mtx));
}

static void	philo_sleep(t_table *table, t_philo *self)
{
	update_last_change(self);
	pthread_mutex_lock(&(table->stdout_mtx));
	printf("%s%li:%li - %d is sleeping...%s\n", KBLU,
		self->last_change.tv_sec, (long)self->last_change.tv_usec / 1000,
		self->id, KNRM);
	pthread_mutex_unlock(&(table->stdout_mtx));
	usleep(table->time_sleep * 1000);
}

void	*philo_init(void *input)
{
	t_action	state;
	t_table		*table;
	t_philo		*self;

	table = get_table();
	// pthread_mutex_lock(&table->mtx);
	self = &table->philos[*(int *)input];
	// pthread_mutex_unlock(&table->mtx);
	pthread_mutex_lock(&self->mtx);
	self->id = *(int *)input + 1;
	self->times_eaten = 0;
	gettimeofday(&self->last_meal, NULL);
	pthread_mutex_unlock(&self->mtx);
	// pthread_mutex_lock(&(table->stdout_mtx));
	// printf("[!] - Creating philosopher %d...\n", self->id);
	// pthread_mutex_unlock(&(table->stdout_mtx));
	state = 0;
	pthread_mutex_lock(&table->mtx);
	while (table->run_simulation == true) // check for number of times eaten if program was launched with according argument
	{
		pthread_mutex_unlock(&table->mtx);
		if (state == THINK)
			philo_think(table, self);
		else if (state == EAT)
			philo_eat(table, self);
		else if (state == SLEEP)
			philo_sleep(table, self);
		if (++state > 2)
			state = 0;
		pthread_mutex_lock(&table->mtx);
	}
	pthread_mutex_unlock(&table->mtx);
	return (NULL);
}
