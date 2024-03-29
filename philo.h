/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 15:55:25 by kipouliq          #+#    #+#             */
/*   Updated: 2024/03/29 14:51:35 by kipouliq         ###   ########.fr       */
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

typedef struct philo
{
	t_mutex				*forks;
	t_mutex				*philo_count_lock;
	t_mutex				*write_lock;
	t_timeval			starting_time;
    int                 philos_deaths[5];
	int					total_philo_nb;
	int					philo_nb;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					time_to_think;
	int					nb_of_meals;
    int                 died_philo;
}						t_philo;

#endif