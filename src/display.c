/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antmoren <antmoren@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 14:45:32 by antmoren          #+#    #+#             */
/*   Updated: 2023/04/11 05:03:30 by antmoren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	print_status_debug(t_philo *philo, char *color, char *str,
		t_status status)
{
	if (status == GOT_FORK_1)
		printf("[%10ld]\t%s%03d\t%s\e[0m: fork [%d]\n",
			get_time_in_ms() - philo->table->start_time,
			color,
			philo->id + 1,
			str,
			philo->fork[0]);
	else if (status == GOT_FORK_2)
		printf("[%10ld]\t%s%03d\t%s\e[0m: fork [%d]\n",
			get_time_in_ms() - philo->table->start_time,
			color,
			philo->id + 1,
			str,
			philo->fork[1]);
	else
		printf("[%10ld]\t%s%03d\t%s\e[0m\n",
			get_time_in_ms() - philo->table->start_time,
			color,
			philo->id + 1,
			str);
}

void	write_status_debug(t_philo *philo, t_status status)
{
	if (status == DIED)
		print_status_debug(philo, RED, "died", status);
	else if (status == EATING)
		print_status_debug(philo, GREEN, "is eating", status);
	else if (status == SLEEPING)
		print_status_debug(philo, CYAN, "is sleeping", status);
	else if (status == THINKING)
		print_status_debug(philo, CYAN, "is thinking", status);
	else if (status == GOT_FORK_1)
		print_status_debug(philo, PURPLE, "has taken a fork", status);
	else if (status == GOT_FORK_2)
		print_status_debug(philo, PURPLE, "has taken a fork", status);
}

void	print_status(t_philo *philo, char *str)
{
	printf("%ld %d %s\n", get_time_in_ms() - philo->table->start_time,
		philo->id + 1, str);
}

void	write_status(t_philo *philo, bool reaper_report, t_status status)
{
	pthread_mutex_lock(&philo->table->write_lock);
	if (has_simulation_stopped(philo->table) == true && reaper_report == false)
	{
		pthread_mutex_unlock(&philo->table->write_lock);
		return ;
	}
	if (DEBUG_FORMATTING == true)
	{
		write_status_debug(philo, status);
		pthread_mutex_unlock(&philo->table->write_lock);
		return ;
	}
	if (status == DIED)
		print_status(philo, "died");
	else if (status == EATING)
		print_status(philo, "is eating");
	else if (status == SLEEPING)
		print_status(philo, "is sleeping");
	else if (status == THINKING)
		print_status(philo, "is thinking");
	else if (status == GOT_FORK_1 || status == GOT_FORK_2)
		print_status(philo, "has taken a fork");
	pthread_mutex_unlock(&philo->table->write_lock);
}

void	write_outcome(t_table *table)
{
	unsigned int	i;
	unsigned int	full_count;
	unsigned int	philo_meals_new;

	full_count = 0;
	i = 0;
	philo_meals_new = (unsigned int)table->n_philo_meals;
	while (i < table->n_of_philo)
	{
		if (table->philosophers[i]->times_ate >= philo_meals_new)
			full_count++;
		i++;
	}
	pthread_mutex_lock(&table->write_lock);
	printf("%d/%d philosophers had at least %d meals.\n",
		full_count,
		table->n_of_philo,
		table->n_philo_meals);
	pthread_mutex_unlock(&table->write_lock);
	return ;
}
