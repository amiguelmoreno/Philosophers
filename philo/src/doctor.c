/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grim_reaper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antmoren <antmoren@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 14:55:58 by antmoren          #+#    #+#             */
/*   Updated: 2023/04/23 21:34:43 by antmoren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	set_sim_stop_flag(t_table *table, bool state)
{
	pthread_mutex_lock(&table->sim_stop_lock);
	table->sim_stop = state;
	pthread_mutex_unlock(&table->sim_stop_lock);
}

bool	has_simulation_stopped(t_table *table)
{
	bool	r;

	r = false;
	pthread_mutex_lock(&table->sim_stop_lock);
	if (table->sim_stop == true)
		r = true;
	pthread_mutex_unlock(&table->sim_stop_lock);
	return (r);
}

static bool	kill_philo(t_philo *philo)
{
	time_t	time;

	time = get_time_in_ms();
	if ((time - philo->last_meal) >= philo->table->t_to_die)
	{
		set_sim_stop_flag(philo->table, true);
		write_status(philo, true, DIED);
		pthread_mutex_unlock(&philo->meal_time_lock);
		return (true);
	}
	return (false);
}

static bool	end_condition_reached(t_table *table)
{
	unsigned int	i;
	bool			all_ate_enough;
	unsigned int	philo_meals_new;

	all_ate_enough = true;
	i = 0;
	philo_meals_new = (unsigned int)table->n_philo_meals;
	while (i < table->n_of_philo)
	{
		pthread_mutex_lock(&table->philosophers[i]->meal_time_lock);
		if (kill_philo(table->philosophers[i]))
			return (true);
		if (table->n_philo_meals != -1)
			if (table->philosophers[i]->times_ate < philo_meals_new)
				all_ate_enough = false;
		pthread_mutex_unlock(&table->philosophers[i]->meal_time_lock);
		i++;
	}
	if (table->n_philo_meals != -1 && all_ate_enough == true)
	{
		set_sim_stop_flag(table, true);
		return (true);
	}
	return (false);
}

void	*doctor(void *data)
{
	t_table	*table;

	table = (t_table *)data;
	if (table->n_philo_meals == 0)
		return (NULL);
	set_sim_stop_flag(table, false);
	sim_start_delay(table->start_time);
	while (true)
	{
		if (end_condition_reached(table) == true)
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
