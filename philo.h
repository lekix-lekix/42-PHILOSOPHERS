/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 15:55:25 by kipouliq          #+#    #+#             */
/*   Updated: 2024/03/26 17:59:35 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <stdio.h>
# include <sys/time.h>
# include <stdlib.h>

typedef struct timeval	t_timeval;

typedef struct time
{
	long int			time_sec;
	float				time_ms;
}						t_time;

typedef struct philo
{
    pthread_mutex_t     *forks;
    int                 philo_nb;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
    int                 time_to_think;
	int					nb_of_meals;
}						t_philo;

#endif