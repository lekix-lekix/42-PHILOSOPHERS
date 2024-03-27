/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 15:55:25 by kipouliq          #+#    #+#             */
/*   Updated: 2024/03/27 18:09:53 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <stdio.h>
# include <sys/time.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct timeval	t_timeval;

typedef struct time
{
	long int			time_sec;
	float				time_ms;
}						t_time;

typedef struct philo
{
    pthread_mutex_t     *forks;
    pthread_mutex_t     *philo_nb_lock;
    int                 total_philo_nb;
    int                 philo_nb;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
    int                 time_to_think;
	int					nb_of_meals;
}						t_philo;

#endif