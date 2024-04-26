/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines_bis.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 16:34:41 by kipouliq          #+#    #+#             */
/*   Updated: 2024/04/25 16:43:42 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

int	solo_philo_routine(t_philo *data)
{
	sem_wait(data->locks->forks);
	printf("%ld %d has taken a fork\n", get_time_elapsed(&data->starting_time),
		1);
	usleep(data->time_to_die * 1000);
	sem_post(data->locks->forks);
	death_routine(data, 1);
	return (0);
}

int	print_fork(t_philo *data, int philo_index)
{
	sem_wait(data->locks->write);
	printf("%ld %d has taken a fork\n", get_time_elapsed(&data->starting_time),
		philo_index);
	sem_post(data->locks->write);
	return (0);
}

int	death_routine(t_philo *data, int philo_index)
{
	sem_wait(data->locks->write);
	printf("%ld %d died\n", get_time_elapsed(&data->starting_time),
		philo_index);
	sem_post(data->locks->deaths);
	exit(-1);
}
