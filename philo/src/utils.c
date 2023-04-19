/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antmoren <antmoren@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 09:36:19 by antmoren          #+#    #+#             */
/*   Updated: 2023/04/19 18:35:51 by antmoren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	*free_table(t_table *table)
{
	unsigned int	i;

	if (!table)
		return (NULL);
	if (table->fork_locks != NULL)
		free(table->fork_locks);
	if (table->philosophers != NULL)
	{
		i = 0;
		while (i < table->n_of_philo)
		{
			if (table->philosophers[i] != NULL)
				free(table->philosophers[i]);
			i++;
		}
		free(table->philosophers);
	}
	free(table);
	return (NULL);
}

int	error_failure(char *message, t_table *table)
{
	if (table != NULL)
		free_table(table);
	printf("\nError\n%s\n", message);
	return (0);
}

void	destroy_all_mutexes(t_table *table)
{
	unsigned int	i;

	i = 0;
	while (i < table->n_of_philo)
	{
		pthread_mutex_destroy(&table->fork_locks[i]);
		pthread_mutex_destroy(&table->philosophers[i]->meal_time_lock);
		i++;
	}
	pthread_mutex_destroy(&table->write_lock);
	pthread_mutex_destroy(&table->sim_stop_lock);
}

void	*error_null(char *message, t_table *table)
{
	if (table != NULL)
		free_table(table);
	printf("\nError\n%s\n", message);
	return (NULL);
}
