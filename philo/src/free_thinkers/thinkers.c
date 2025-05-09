#include "philosophers.h"

/**
 * @brief Makes a philosopher eat by making him pick up his two forks
 * (mutexes) and waiting for `time_eat` milliseconds.
 */
static void	philo_eat(t_table *table, t_philo *self)
{
	if (pickup_forks(table, self) == -1)
		return ;
	pthread_mutex_lock(&table->mtx);
	if (table->run_simulation == false)
	{
		pthread_mutex_unlock(&table->mtx);
		pthread_mutex_unlock(&self->left_fork->mtx);
		pthread_mutex_unlock(&self->right_fork->mtx);
		return ;
	}
	pthread_mutex_unlock(&table->mtx);
	pthread_mutex_lock(&self->mtx);
	if (gettimeofday(&self->last_meal, NULL) == -1)
		sfwrite_stderr("[!] - Failed to get time of day!\n");
	pthread_mutex_unlock(&self->mtx);
	update_last_change(self);
	pthread_mutex_lock(&(table->stdout_mtx));
	pthread_mutex_lock(&self->mtx);
	printf("%s%lu - %d is eating%s\n", KYEL,
		get_timestamp(self->last_change, table), self->id, KNRM);
	pthread_mutex_unlock(&self->mtx);
	pthread_mutex_unlock(&(table->stdout_mtx));
	usleep(table->time_eat * 1000);
	self->left_fork->last_eater = self;
	self->right_fork->last_eater = self;
	pthread_mutex_unlock(&self->left_fork->mtx);
	pthread_mutex_unlock(&self->right_fork->mtx);
	pthread_mutex_lock(&(table->stdout_mtx));
	printf("[!] - Philo %d dropped his forks!\n", self->id);
	pthread_mutex_unlock(&(table->stdout_mtx));
	pthread_mutex_lock(&self->mtx);
	self->times_eaten++;
	self->action = SLEEP;
	pthread_mutex_unlock(&self->mtx);
}

/**
 * @brief Philosophers think while they are waiting for forks.
 * This just writes the log message for thinking before transitioning to
 * EATing.
 */
static void	philo_think(t_table *table, t_philo *self)
{
	update_last_change(self);
	pthread_mutex_lock(&(table->stdout_mtx));
	pthread_mutex_lock(&self->mtx);
	printf("%s%lu - %d is thinking%s\n", KGRN,
		get_timestamp(self->last_change, table), self->id, KNRM);
	pthread_mutex_unlock(&self->mtx);
	pthread_mutex_unlock(&(table->stdout_mtx));
	self->action = EAT;
}

/**
 * @brief After eating, a philosopher sleeps for `time_sleep` milliseconds.
 */
static void	philo_sleep(t_table *table, t_philo *self)
{
	update_last_change(self);
	pthread_mutex_lock(&(table->stdout_mtx));
	pthread_mutex_lock(&self->mtx);
	printf("%s%lu - %d is sleeping%s\n", KBLU,
		get_timestamp(self->last_change, table), self->id, KNRM);
	pthread_mutex_unlock(&self->mtx);
	pthread_mutex_unlock(&(table->stdout_mtx));
	usleep(table->time_sleep * 1000);
	self->action = THINK;
}

/**
 * @brief Main philosophers loop, thinking, eating and sleeping.
 */
void	philo_run(t_table *table, t_philo *self)
{
	self->action = EAT;
	pthread_mutex_lock(&table->mtx);
	while (table->run_simulation == true)
	{
		pthread_mutex_unlock(&table->mtx);
		if (table->philo_count == 1) // TODO: hardcoded fix, maybe there's a better way?
			;
		else if (self->action == THINK)
			philo_think(table, self);
		else if (self->action == EAT)
			philo_eat(table, self);
		else if (self->action == SLEEP)
			philo_sleep(table, self);
		pthread_mutex_lock(&table->mtx);
	}
	pthread_mutex_unlock(&table->mtx);
}

// TODO : better logic to pick up forks (even and uneven stuff, look it up)
/**
 * @brief Initializes a new thread as a philosopher. Sets up its struct and
 * launches the main loop.
 *
 * @param input Input passed down from pthreads_create, expected to be NULL.
 * @return NULL
 */
void	*philo_init(void *input)
{
	t_table		*table;
	t_philo		*self;
	static int	id;

	(void)input;
	table = get_table();
	pthread_mutex_lock(&table->index_mtx);
	self = &table->philos[id];
	id++;
	pthread_mutex_lock(&self->mtx);
	self->id = id;
	pthread_mutex_unlock(&table->index_mtx);
	self->times_eaten = 0;
	self->last_meal = table->start;
	self->left_fork = &table->forks[self->id - 1];
	if (self->id == table->philo_count)
		self->right_fork = &table->forks[0];
	else
		self->right_fork = &table->forks[self->id];
	pthread_mutex_unlock(&self->mtx);
	pthread_mutex_lock(&(table->stdout_mtx));
	printf("[!] - Philo %d is born!\n", self->id);
	pthread_mutex_unlock(&(table->stdout_mtx));
	philo_run(table, self);
	return (NULL);
}
