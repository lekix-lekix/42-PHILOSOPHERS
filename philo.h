/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 15:55:25 by kipouliq          #+#    #+#             */
/*   Updated: 2024/04/11 12:29:23 by kipouliq         ###   ########.fr       */
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

typedef struct philo
{
	t_mutex				*forks;
	t_mutex				*philo_count_lock;
	t_mutex				*write_lock;
    t_mutex             *deaths_lock;
    t_mutex             *f_meals_lock;
    t_mutex             *nb_of_meals_lock;
    t_mutex             *end_exec_lock;
	t_timeval			starting_time;
    int                 *philo_deaths;
    int                 *philo_f_meals;
	int					total_philo_nb;
	int					philo_nb;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					time_to_think;
	int					nb_of_meals;
    int                 died_philo;
    int                 end_exec;
}						t_philo;

#endif