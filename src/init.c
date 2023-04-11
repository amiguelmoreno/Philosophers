/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antmoren <antmoren@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 12:44:15 by antmoren          #+#    #+#             */
/*   Updated: 2023/04/11 05:01:27 by antmoren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

pthread_mutex_t	*init_forks(t_table *table)
{
	pthread_mutex_t	*forks;
	unsigned int	i;

	forks = malloc(sizeof(pthread_mutex_t) * table->n_of_philo);
	if (!forks)
		return (error_null(ERR_MALLOC, table));
	i = 0;
	while (i < table->n_of_philo)
	{
		if (pthread_mutex_init(&forks[i], 0) != 0)
			return (error_null(ERR_MUTEX, table));
		i++;
	}
	return (forks);
}

void	assign_forks(t_philo *philo)
{
	philo->fork[0] = philo->id;
	philo->fork[1] = (philo->id + 1) % philo->table->n_of_philo;
	if (philo->id % 2)
	{
		philo->fork[0] = (philo->id + 1) % philo->table->n_of_philo;
		philo->fork[1] = philo->id;
	}
}

t_philo	**init_philosophers(t_table *table)
{
	t_philo			**philosophers;
	unsigned int	i;

	philosophers = malloc(sizeof(t_philo) * table->n_of_philo);
	if (!philosophers)
		return (error_null(ERR_MALLOC, table));
	i = 0;
	while (i < table->n_of_philo)
	{
		philosophers[i] = malloc(sizeof(t_philo) * 1);
		if (!philosophers[i])
			return (error_null(ERR_MALLOC, table));
		if (pthread_mutex_init(&philosophers[i]->meal_time_lock, NULL) != 0)
			return (error_null(ERR_MUTEX, table));
		philosophers[i]->table = table;
		philosophers[i]->id = i;
		philosophers[i]->times_ate = 0;
		assign_forks(philosophers[i]);
		i++;
	}
	return (philosophers);
}

bool	init_global_mutexes(t_table *table)
{
	table->fork_locks = init_forks(table);
	if (!table->fork_locks)
		return (false);
	if (pthread_mutex_init(&table->sim_stop_lock, NULL) != 0)
		return (error_null(ERR_MUTEX, table));
	if (pthread_mutex_init(&table->write_lock, NULL) != 0)
		return (error_null(ERR_MUTEX, table));
	return (true);
}

t_table	*init_table(int ac, char **av, int i)
{
	t_table	*table;

	table = malloc(sizeof(t_table) * 1);
	if (!table)
		return (error_null(ERR_MALLOC, table));
	table->n_of_philo = integer_atoi(av[i++]);
	table->t_to_die = integer_atoi(av[i++]);
	table->t_to_eat = integer_atoi(av[i++]);
	table->t_to_sleep = integer_atoi(av[i++]);
	table->n_philo_meals = -1;
	if (ac - 1 == 5)
		table->n_philo_meals = integer_atoi(av[i]);
	table->philosophers = init_philosophers(table);
	if (!table->philosophers)
		return (NULL);
	if (!init_global_mutexes(table))
		return (NULL);
	table->sim_stop = false;
	return (table);
}
