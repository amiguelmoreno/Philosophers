/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antmoren <antmoren@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 20:28:37 by antmoren          #+#    #+#             */
/*   Updated: 2023/04/19 18:35:50 by antmoren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

bool	start_simulation(t_table *table)
{
	unsigned int	i;

	table->start_time = get_time_in_ms() + (table->n_of_philo * 2 * 10);
	i = 0;
	while (i < table->n_of_philo)
	{
		if (pthread_create(&table->philosophers[i]->thread, NULL, &actions,
				table->philosophers[i]) != 0)
			return (error_failure(ERR_THREAD, table));
		i++;
	}
	if (table->n_of_philo > 1)
	{
		if (pthread_create(&table->grim_reaper, NULL, &grim_reaper, table) != 0)
			return (error_failure(ERR_THREAD, table));
	}
	return (true);
}

void	stop_simulation(t_table *table)
{
	unsigned int	i;

	i = 0;
	while (i < table->n_of_philo)
	{
		pthread_join(table->philosophers[i]->thread, NULL);
		i++;
	}
	if (table->n_of_philo > 1)
		pthread_join(table->grim_reaper, NULL);
	destroy_all_mutexes(table);
	free_table(table);
}

int	main(int argc, char **argv)
{
	t_table	*table;

	table = NULL;
	if (argc - 1 < 4 || argc - 1 > 5)
		return (error_failure(ERR_ARGS_NUMBER, table));
	if (!check_args(argc, argv, table))
		return (EXIT_FAILURE);
	table = init_table(argc, argv, 1);
	if (!table)
		return (EXIT_FAILURE);
	if (!start_simulation(table))
		return (EXIT_FAILURE);
	stop_simulation(table);
	return (EXIT_SUCCESS);
}
