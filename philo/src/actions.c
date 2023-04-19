/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antmoren <antmoren@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 14:34:12 by antmoren          #+#    #+#             */
/*   Updated: 2023/04/19 18:35:33 by antmoren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	philo_do_action(t_table *table, time_t sleep_time)
{
	time_t	time_finish_action;

	time_finish_action = get_time_in_ms() + sleep_time;
	while (get_time_in_ms() < time_finish_action)
	{
		if (has_simulation_stopped(table))
			break ;
		usleep(100);
	}
}

void	eat_sleep_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_locks[philo->fork[0]]);
	write_status(philo, false, GOT_FORK_1);
	pthread_mutex_lock(&philo->table->fork_locks[philo->fork[1]]);
	write_status(philo, false, GOT_FORK_2);
	write_status(philo, false, EATING);
	pthread_mutex_lock(&philo->meal_time_lock);
	philo->last_meal = get_time_in_ms();
	pthread_mutex_unlock(&philo->meal_time_lock);
	philo_do_action(philo->table, philo->table->t_to_eat);
	if (has_simulation_stopped(philo->table) == false)
	{
		pthread_mutex_lock(&philo->meal_time_lock);
		philo->times_ate += 1;
		pthread_mutex_unlock(&philo->meal_time_lock);
	}
	write_status(philo, false, SLEEPING);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->fork[1]]);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->fork[0]]);
	philo_do_action(philo->table, philo->table->t_to_sleep);
}

void	think_routine(t_philo *philo, bool silent)
{
	time_t	t_to_think;

	pthread_mutex_lock(&philo->meal_time_lock);
	t_to_think = (philo->table->t_to_die - (get_time_in_ms() - philo->last_meal)
			- philo->table->t_to_eat) / 2;
	pthread_mutex_unlock(&philo->meal_time_lock);
	if (t_to_think < 0)
		t_to_think = 0;
	if (t_to_think == 0 && silent == true)
		t_to_think = 1;
	if (t_to_think > 600)
		t_to_think = 200;
	if (silent == false)
		write_status(philo, false, THINKING);
	philo_do_action(philo->table, t_to_think);
}

void	*one_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_locks[philo->fork[0]]);
	write_status(philo, false, GOT_FORK_1);
	philo_do_action(philo->table, philo->table->t_to_die);
	write_status(philo, false, DIED);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->fork[0]]);
	return (NULL);
}

void	*actions(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if (philo->table->n_philo_meals == 0)
		return (NULL);
	pthread_mutex_lock(&philo->meal_time_lock);
	philo->last_meal = philo->table->start_time;
	pthread_mutex_unlock(&philo->meal_time_lock);
	sim_start_delay(philo->table->start_time);
	if (philo->table->t_to_die == 0)
		return (NULL);
	if (philo->table->n_of_philo == 1)
		return (one_philo_routine(philo));
	else if (philo->id % 2)
		think_routine(philo, true);
	while (has_simulation_stopped(philo->table) == false)
	{
		eat_sleep_routine(philo);
		think_routine(philo, false);
	}
	return (NULL);
}
