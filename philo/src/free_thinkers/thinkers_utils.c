#include "philosophers.h"

/**
 * @brief Locks the given philosophers' mutex and updates its last_change
 * timestamp.
 */
void	update_last_change(t_philo *self)
{
	if (gettimeofday(&self->last_change, NULL) == -1)
		sfwrite_stderr("[!] - Failed to get time of day!\n");
}

/**
 * @brief Pick up both the left and right forks of a philosopher.
 *
 * @return 0 if everything can proceed, -1 if a fork was last picked up
 * by self, meaning we have to let someone else take it.
 */
int pickup_forks(t_table *table, t_philo *self)
{
	pthread_mutex_lock(&self->left_fork->mtx);
	if (self->left_fork->last_eater == self)
	{
		pthread_mutex_unlock(&self->left_fork->mtx);
		return (-1);
	}
	pthread_mutex_lock(&(table->stdout_mtx));
	printf("[!] - Philo %d picked up his left fork!\n", self->id);
	pthread_mutex_unlock(&(table->stdout_mtx));
	pthread_mutex_lock(&self->right_fork->mtx);
	if (self->right_fork->last_eater == self)
	{
		pthread_mutex_unlock(&self->left_fork->mtx);
		pthread_mutex_unlock(&self->right_fork->mtx);
		return (-1);
	}
	pthread_mutex_lock(&(table->stdout_mtx));
	printf("[!] - Philo %d picked up his right fork!\n", self->id);
	pthread_mutex_unlock(&(table->stdout_mtx));
	return (0);
}
