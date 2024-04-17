/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 15:55:25 by kipouliq          #+#    #+#             */
/*   Updated: 2024/04/17 17:39:13 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H
# include <fcntl.h>
# include <pthread.h>
# include <semaphore.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct timeval	t_timeval;

typedef struct routine_data
{
	int					philo_index;
	int					meals_finished;
	int					max_meals;
	long int			last_meal;
}						t_routine_data;

typedef struct philo
{
	sem_t				*sem_forks;
    sem_t               *sem_philo_index;
	t_timeval			starting_time;
	int					*philo_deaths;
	int					*philo_f_meals;
    int                 *philos_pids;
    int                 philo_index;
	int					total_philo_nb;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					time_to_think;
	int					nb_of_meals;
	int					died_philo;
}						t_philo;

int						check_args(char **argv);
char					*ft_itoa(int n);
int						ft_strlen(char *str);
int						is_digit(char c);

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

int						ft_atoi(const char *nptr);
int						sleep_routine(t_philo *data, long int *last_meal,
							int philo_index);
int						think_routine(t_philo *data, long int *last_meal,
							int philo_index);
int						monitor_philos(t_philo *data);
int						ft_free_struct(t_philo *data);
int						check_data(t_philo *data, int argc);
int						fill_death_tab(t_philo *data);

#endif