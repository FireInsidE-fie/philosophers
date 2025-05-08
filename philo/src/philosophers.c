/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fireinside <aisling.fontaine@pm.me>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 19:16:56 by fireinside        #+#    #+#             */
/*   Updated: 2025/04/08 19:16:56 by fireinside       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/**
 * @brief Checks input for anything other than digits.
 *
 * @return 0 if input is fine, 1 if not.
 */
static int	check_input(int argc, char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

int	main(const int argc, char **argv)
{
	t_table	*table;

	if (argc < 5 || argc > 6)
		return (printf("[!] - Usage: ./philosophers number_of_philosophers "
				 "time_to_die time_to_eat time_to_sleep"
				 " [number_of_times_each_philosopher_must_eat]"), 1);
	if (check_input(argc, argv) == 1)
		return (printf("[!] - Please only provide positive"
				 " integers as arguments.\n"), 1);
	if (init_table(argv) == 1)
		return (1);
	table = get_table();
	printf("[!] - Philo count = %d\n", table->philo_count);
	init_mutexes();
	launch_threads();
	wait_on_threads();
	clean_table();
	return (0);
}
