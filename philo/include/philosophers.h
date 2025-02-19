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
# include <inttypes.h>
# include <threads.h>
# include <pthread.h>

typedef struct s_philo
{
	__thrd_t	t;
}	t_philo;

typedef struct s_fork
{
	int8_t	status;
	mtx_t	mtx;
}	t_fork;

#endif //PHILOSOPHERS_H
