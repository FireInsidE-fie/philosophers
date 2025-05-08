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

/**
 * @brief Locks the given philosophers' mutex and updates its last_change
 * timestamp.
 */
void	update_last_change(t_philo *self)
{
	pthread_mutex_lock(&self->mtx);
	if (gettimeofday(&self->last_change, NULL) == -1)
		sfwrite_stderr("[!] - Failed to get time of day!\n");
	pthread_mutex_unlock(&self->mtx);
}

static void	philo_eat(t_table *table, t_philo *self)
{
	pthread_mutex_lock(&self->left_fork->mtx);
	pthread_mutex_lock(&(table->stdout_mtx));
	printf("[!] - Philo %d picked up his left fork!\n", self->id);
	pthread_mutex_unlock(&(table->stdout_mtx));
	pthread_mutex_lock(&self->right_fork->mtx);
	pthread_mutex_lock(&(table->stdout_mtx));
	printf("[!] - Philo %d picked up his right fork!\n", self->id);
	pthread_mutex_unlock(&(table->stdout_mtx));
	if (table->run_simulation == false)
	{
		pthread_mutex_unlock(&self->left_fork->mtx);
		pthread_mutex_unlock(&self->right_fork->mtx);
		return ;
	}
	pthread_mutex_lock(&self->mtx);
	if (gettimeofday(&self->last_meal, NULL) == -1)
		sfwrite_stderr("[!] - Failed to get time of day!\n");
	pthread_mutex_unlock(&self->mtx);
	update_last_change(self);
	pthread_mutex_lock(&(table->stdout_mtx));
	printf("%s%li:%li - %d is eating%s\n", KYEL,
			self->last_change.tv_sec, (long)self->last_change.tv_usec / 1000,
			self->id, KNRM);
	pthread_mutex_unlock(&(table->stdout_mtx));
	usleep(table->time_eat * 1000);
	self->left_fork->last_eater = self;
	self->right_fork->last_eater = self;
	pthread_mutex_unlock(&self->left_fork->mtx);
	pthread_mutex_unlock(&self->right_fork->mtx);
	pthread_mutex_lock(&(table->stdout_mtx));
	printf("[!] - Philo %d dropped his forks!\n", self->id);
	pthread_mutex_unlock(&(table->stdout_mtx));
	self->times_eaten++;
	self->action = SLEEP;
}

/**
 * @brief Philosophers think while they are waiting for forks.
 */
static void	philo_think(t_table *table, t_philo *self)
{
	update_last_change(self);
	pthread_mutex_lock(&(table->stdout_mtx));
	printf("%s%li:%li - %d is thinking%s\n", KGRN,
			self->last_change.tv_sec, (long)self->last_change.tv_usec / 1000,
			self->id, KNRM);
	pthread_mutex_unlock(&(table->stdout_mtx));
	self->action = EAT;
}

static void	philo_sleep(t_table *table, t_philo *self)
{
	update_last_change(self);
	pthread_mutex_lock(&(table->stdout_mtx));
	printf("%s%li:%li - %d is sleeping%s\n", KBLU,
			self->last_change.tv_sec, (long)self->last_change.tv_usec / 1000,
			self->id, KNRM);
	pthread_mutex_unlock(&(table->stdout_mtx));
	usleep(table->time_sleep * 1000);
	self->action = THINK;
}

void	*philo_init(void *input)
{
	t_table *table;
	t_philo *self;
	int id;

	id = *(int*)input;
	table = get_table();
	self = &table->philos[id];
	pthread_mutex_lock(&self->mtx);
	self->id = *(int*)input + 1;
	self->times_eaten = 0;
	gettimeofday(&self->last_meal, NULL);
	self->left_fork = &table->forks[self->id - 1];
	if (self->id == table->philo_count)
		self->right_fork = &table->forks[0];
	else
		self->right_fork = &table->forks[self->id];
	pthread_mutex_unlock(&self->mtx);
	self->action = THINK;
	pthread_mutex_lock(&table->mtx);
	// TODO check for number of times eaten if program was launched with according argument
	while (table->run_simulation == true)
	{
		pthread_mutex_unlock(&table->mtx);
		if (self->action == THINK)
			philo_think(table, self);
		else if (self->action == EAT)
			philo_eat(table, self);
		else if (self->action == SLEEP)
			philo_sleep(table, self);
		pthread_mutex_lock(&table->mtx);
	}
	pthread_mutex_unlock(&table->mtx);
	return (NULL);
}
