/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 15:55:25 by kipouliq          #+#    #+#             */
/*   Updated: 2024/04/15 18:20:27 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct timeval	t_timeval;
typedef pthread_mutex_t	t_mutex;

typedef struct mutex_pair
{
	t_mutex				*fork_1;
	t_mutex				*fork_2;
}						t_mutex_pair;

typedef struct routine_data
{
	int					philo_index;
	int					meals_finished;
	int					max_meals;
	long int			last_meal;
}						t_routine_data;

typedef struct philo
{
	t_mutex				*forks;
	t_mutex				*philo_count_lock;
	t_mutex				*write_lock;
	t_mutex				*deaths_lock;
	t_mutex				*f_meals_lock;
	t_mutex				*nb_of_meals_lock;
	t_mutex				*end_exec_lock;
	t_timeval			starting_time;
	int					*philo_deaths;
	int					*philo_f_meals;
	int					total_philo_nb;
	int					philo_nb;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					time_to_think;
	int					nb_of_meals;
	int					died_philo;
}						t_philo;

int						philo_routine(t_philo *data);
int						check_end_exec(t_philo *data, int philo_index);
int						init_philo_data(int argc, char **argv, t_philo *data);
int						init_tabs(t_philo *data);
int						are_all_philos_fed(t_philo *data);
long int				get_time_elapsed(t_timeval *starting_time);
int						is_even(int nb);
int						usleep_and_check(t_philo *data, long int *last_meal,
							int time, int philo_index);
int						check_time_to_die(long int *last_meal,
							t_timeval starting_time, int ttd);
int						only_one_philo_routine(t_philo *data);
int						eat_and_check(t_philo *data, int philo_index);
int						print_fork(t_philo *data, int philo_index);
int						death_routine(t_philo *data, t_mutex *fork_1,
							t_mutex *fork_2, int philo_index);
int						ft_atoi(const char *nptr);
t_mutex					*create_mutex_tab(int mtx_nb);
int						free_everything(t_philo *data);
int						wait_philos(pthread_t *philos_tab, int philos_nb);
int						start_meal(t_mutex_pair *forks, t_philo *data,
							long int *last_meal, int philo_index);
int						sleep_routine(t_philo *data, long int *last_meal,
							int philo_index);
int						think_routine(t_philo *data, long int *last_meal,
							int philo_index);
int						monitor_philos(t_philo *data);
int						ft_free_struct(t_philo *data);
int						check_data(t_philo *data, int argc);
int						fill_death_tab(t_philo *data);

#endif