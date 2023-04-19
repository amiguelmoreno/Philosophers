/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antmoren <antmoren@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 12:19:43 by antmoren          #+#    #+#             */
/*   Updated: 2023/04/19 18:35:48 by antmoren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#define ERR_ARGS_NUMBER "Missing or exceding arguments."
#define ERR_INPUT_DIGIT "Arguments must be integers between 0 and 2147483647."
#define ERR_INPUT_PHILO "Number of philosophers must be between 1 and 250."
#define ERR_THREAD "Could not create thread."
#define ERR_MALLOC "Could not allocate memory."
#define ERR_MUTEX "Could not create mutex."

typedef struct s_philo	t_philo;
typedef struct s_table
{
	time_t				start_time;
	unsigned int		n_of_philo;
	time_t				t_to_die;
	time_t				t_to_eat;
	time_t				t_to_sleep;
	int					n_philo_meals;
	t_philo				**philosophers;
	pthread_mutex_t		sim_stop_lock;
	pthread_mutex_t		write_lock;
	pthread_mutex_t		*fork_locks;
	pthread_t			grim_reaper;
	bool				sim_stop;
}						t_table;

typedef struct s_philo
{
	pthread_t			thread;
	unsigned int		id;
	unsigned int		times_ate;
	unsigned int		fork[2];
	pthread_mutex_t		meal_time_lock;
	time_t				last_meal;
	t_table				*table;
}						t_philo;

typedef enum e_status
{
	DIED = 0,
	EATING = 1,
	SLEEPING = 2,
	THINKING = 3,
	GOT_FORK_1 = 4,
	GOT_FORK_2 = 5
}						t_status;

/* Input_Checker */
bool					check_args(int argc, char **args, t_table *table);
int						integer_atoi(char *str);

/* Utils */
int						error_failure(char *message, t_table *table);
void					*error_null(char *message, t_table *table);
void					*free_table(t_table *table);
void					destroy_all_mutexes(t_table *table);

/* Init */
t_table					*init_table(int ac, char **av, int i);

/* Grim Reaper */
void					*grim_reaper(void *data);
bool					has_simulation_stopped(t_table *table);

/* Actions */
void					*actions(void *data);

/* Time */
void					sim_start_delay(time_t start_time);
void					philo_sleep(t_table *table, time_t sleep_time);
time_t					get_time_in_ms(void);

/* Display */
void					write_outcome(t_table *table);
void					write_status(t_philo *philo, bool reaper_report,
							t_status status);