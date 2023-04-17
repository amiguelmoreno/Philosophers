/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antmoren <antmoren@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 10:34:34 by antmoren          #+#    #+#             */
/*   Updated: 2023/03/21 17:39:48 by antmoren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

bool	contains_only_digits(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		i++;
	}
	return (true);
}

int	integer_atoi(char *str)
{
	unsigned long long int	nb;
	int						i;

	i = 0;
	nb = 0;
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
	{
		nb = nb * 10 + (str[i] - '0');
		i++;
	}
	if (nb > INT_MAX)
		return (-1);
	return ((int)nb);
}

bool	check_args(int argc, char **args, t_table *table)
{
	int	i;
	int	nb;

	i = 1;
	while (i < argc)
	{
		if (!contains_only_digits(args[i]))
			return (error_failure(ERR_INPUT_DIGIT, table));
		nb = integer_atoi(args[i]);
		if (i == 1 && (nb <= 0 || nb > 250))
			return (error_failure(ERR_INPUT_PHILO, table));
		if (i != 1 && nb == -1)
			return (error_failure(ERR_INPUT_DIGIT, table));
		i++;
	}
	return (true);
}
