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

static void	update_last_change(t_philo *self)
{
	if (gettimeofday(&self->last_change, NULL) == -1)
		sfwrite_stderr("[!] - Failed to get time of day!\n");
}

static void	philo_eat(t_table *table, t_philo *self)
{
	update_last_change(self);
	pthread_mutex_lock(&(table->stdout_mtx));
	printf("%s%li:%li - %d is eating...%s\n", KYEL,
		self->last_change.tv_sec, self->last_change.tv_usec / 1000,
		self->id, KNRM);
	pthread_mutex_unlock(&(table->stdout_mtx));
	usleep(table->time_eat * 1000);
}

static void	philo_think(t_table *table, t_philo *self)
{
	update_last_change(self);
	pthread_mutex_lock(&(table->stdout_mtx));
	printf("%s%li:%li - %d is thinking...%s\n", KGRN,
		self->last_change.tv_sec, self->last_change.tv_usec / 1000,
		self->id, KNRM);
	pthread_mutex_unlock(&(table->stdout_mtx));
}

static void	philo_sleep(t_table *table, t_philo *self)
{
	update_last_change(self);
	pthread_mutex_lock(&(table->stdout_mtx));
	printf("%s%li:%li - %d is sleeping...%s\n", KBLU,
		self->last_change.tv_sec, self->last_change.tv_usec / 1000,
		self->id, KNRM);
	pthread_mutex_unlock(&(table->stdout_mtx));
	usleep(table->time_sleep * 1000);
}

void	*philo_init(void *input)
{
	t_table	*table;
	t_philo *self;

	table = get_table();
	self = &table->philos[*(int *)input];
	self->id = *(int *)input;
	self->alive = true;
	self->times_eaten = 0;
	pthread_mutex_lock(&(table->stdout_mtx));
	printf("[!] - Creating philosopher %d...\n", self->id);
	pthread_mutex_unlock(&(table->stdout_mtx));
	while (self->alive == true) // check for number of times eaten if program was launched with according argument
	{
		philo_eat(table, self);
		philo_think(table, self);
		philo_sleep(table, self);
	}
	if (self->alive == false)
	{
		pthread_mutex_lock(&(table->stdout_mtx));
		printf("%s%li:%li - %d has died...%s\n", KRED,
			self->last_change.tv_sec, self->last_change.tv_usec / 1000,
			self->id, KNRM);
		pthread_mutex_unlock(&(table->stdout_mtx));
	}
	pthread_exit(NULL);
}
