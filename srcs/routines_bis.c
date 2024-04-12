/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines_bis.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 13:51:21 by kipouliq          #+#    #+#             */
/*   Updated: 2024/04/12 14:50:43 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	only_one_philo_routine(t_philo *data)
{
	t_mutex	*fork;

	fork = &data->forks[0];
	pthread_mutex_lock(fork);
	print_fork(data, 1);
	usleep(data->time_to_die * 1000);
	death_routine(data, NULL, NULL, 0);
	pthread_mutex_unlock(fork);
	return (0);
}
