/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: estettle <estettle@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:12:03 by estettle          #+#    #+#             */
/*   Updated: 2025/02/19 14:10:48 by estettle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdarg.h>
# include <unistd.h>
# include <string.h>
# include <inttypes.h>
# include <threads.h>
# include <pthread.h>

typedef struct s_table
{
	pthread_mutex_t	stdout_mtx;
	pthread_mutex_t	stderr_mtx;
}	t_table;

typedef struct s_philo
{
	__thrd_t	t;
}	t_philo;

typedef struct s_fork
{
	int8_t			status;
	pthread_mutex_t	mtx;
}	t_fork;

t_table	*get_table(void);

// General utils - utils.c
size_t	ft_strlen(char *str);

// Mutex-locked utils - safe_utils.c
int		swrite_stdout(char *buf);
int		swrite_stderr(char *buf);

#endif //PHILOSOPHERS_H
