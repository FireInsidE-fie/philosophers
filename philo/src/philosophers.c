#include "philosophers.h"

/**
 * @brief Checks input for anything other than digits.
 *
 * @return 0 if input is fine, 1 if there's characters other than numbers, 2 if
 * a number is too big.
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
			if ((i == 1 && j > 1) || j > 8)
				return (2);
			j++;
		}
		i++;
	}
	return (0);
}

int	main(const int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		return (printf("[!] - Usage: ./philosophers number_of_philosophers "
				"time_to_die time_to_eat time_to_sleep"
				" [number_of_times_each_philosopher_must_eat]"), 1);
	if (check_input(argc, argv) == 1)
		return (printf("[!] - Please only provide positive"
				" integers as arguments.\n"), 1);
	if (check_input(argc, argv) == 2)
		return (printf("[!] - really? please provide sensible values\n"), 1);
	if (init_table(argv) == 1)
		return (1);
	init_mutexes();
	launch_threads();
	wait_on_threads();
	clean_table();
	return (0);
}
