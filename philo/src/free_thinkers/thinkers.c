#include "philosophers.h"

/**
 * @brief Makes a philosopher eat by making him pick up his two forks
 * (mutexes) and waiting for `time_eat` milliseconds.
 */
static void	philo_eat(t_table *table, t_philo *self)
{
	if (self->id % 2 == 0 && pickup_forks_even(self) == -1
		&& usleep(1000) == 0)
		return ;
	if (self->id % 2 != 0 && pickup_forks_uneven(self) == -1
		&& usleep(1000) == 0)
		return ;
	pthread_mutex_lock(&table->mtx);
	if (table->run_simulation == false)
		return (pthread_mutex_unlock(&table->mtx), drop_forks(self));
	pthread_mutex_unlock(&table->mtx);
	update_last_change(self);
	pthread_mutex_lock(&self->mtx);
	self->last_meal = self->last_change;
	pthread_mutex_lock(&table->stdout_mtx);
	printf("%s%lu - %d is eating%s\n", KYEL,
		get_timestamp(self->last_change, table), self->id, KNRM);
	pthread_mutex_unlock(&(table->stdout_mtx));
	self->times_eaten++;
	self->action = SLEEP;
	pthread_mutex_unlock(&self->mtx);
	usleep(table->time_eat * 1000);
	self->left_fork->last_eater = self;
	self->right_fork->last_eater = self;
	drop_forks(self);
}

/**
 * @brief Philosophers think while they are waiting for forks.
 * This just writes the log message for thinking before transitioning to
 * EATing.
 */
static void	philo_think(t_table *table, t_philo *self)
{
	update_last_change(self);
	pthread_mutex_lock(&self->mtx);
	pthread_mutex_lock(&(table->stdout_mtx));
	printf("%s%lu - %d is thinking%s\n", KGRN,
		get_timestamp(self->last_change, table), self->id, KNRM);
	pthread_mutex_unlock(&(table->stdout_mtx));
	pthread_mutex_unlock(&self->mtx);
	self->action = EAT;
}

/**
 * @brief After eating, a philosopher sleeps for `time_sleep` milliseconds.
 */
static void	philo_sleep(t_table *table, t_philo *self)
{
	update_last_change(self);
	pthread_mutex_lock(&self->mtx);
	pthread_mutex_lock(&(table->stdout_mtx));
	printf("%s%lu - %d is sleeping%s\n", KBLU,
		get_timestamp(self->last_change, table), self->id, KNRM);
	pthread_mutex_unlock(&(table->stdout_mtx));
	pthread_mutex_unlock(&self->mtx);
	usleep(table->time_sleep * 1000);
	self->action = THINK;
}

/**
 * @brief Main philosophers loop, thinking, eating and sleeping.
 */
void	*philo_run(void *input)
{
	t_table		*table;
	t_philo		*self;

	table = get_table();
	self = input;
	pthread_mutex_lock(&table->mtx);
	while (table->run_simulation == true)
	{
		pthread_mutex_unlock(&table->mtx);
		if (table->philo_count == 1)
			usleep(1000);
		else if (self->action == THINK)
			philo_think(table, self);
		else if (self->action == EAT)
			philo_eat(table, self);
		else if (self->action == SLEEP)
			philo_sleep(table, self);
		pthread_mutex_lock(&table->mtx);
	}
	return (pthread_mutex_unlock(&table->mtx), input);
}

/**
 * @brief Initializes all the philosophers structs.
 *
 * @return NULL
 */
void	philos_init(void)
{
	uint32_t	i;
	t_table		*table;

	i = 0;
	table = get_table();
	while (i < table->philo_count)
	{
		table = get_table();
		table->philos[i].times_eaten = 0;
		table->philos[i].last_meal = table->start;
		table->philos[i].left_fork = &table->forks[i];
		table->philos[i].right_fork = &table->forks[(i + 1) % table->philo_count];
		table->philos[i].id = i + 1;
		i++;
	}
}
