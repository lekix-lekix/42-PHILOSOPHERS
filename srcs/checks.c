/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 14:05:58 by kipouliq          #+#    #+#             */
/*   Updated: 2024/04/16 16:33:26 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	check_end_exec(t_philo *data, int philo_index)
{
	pthread_mutex_lock(&data->deaths_lock[philo_index - 1]);
	if (data->philo_deaths[philo_index - 1])
	{
		pthread_mutex_unlock(&data->deaths_lock[philo_index - 1]);
		return (1);
	}
	pthread_mutex_unlock(&data->deaths_lock[philo_index - 1]);
	return (0);
}

int	check_time_to_die(long int *last_meal, t_timeval starting_time, int ttd)
{
	long int	current_time;

	current_time = get_time_elapsed(&starting_time);
	if (current_time - *last_meal >= ttd)
		return (0);
	return (1);
}

int	usleep_and_check(t_philo *data, long int *last_meal, int time,
		int philo_index)
{
	long int	starting_time;
	int			check_sleep;

	starting_time = get_time_elapsed(&data->starting_time);
	check_sleep = 0;
	while (get_time_elapsed(&data->starting_time) - starting_time + 10 < time)
	{
		usleep(10000);
		if (!check_time_to_die(last_meal, data->starting_time,
				data->time_to_die))
			return (-1);
		if (check_end_exec(data, philo_index))
			return (-1);
		check_sleep = get_time_elapsed(&data->starting_time) - starting_time;
	}
	if (time - check_sleep < time && (time - check_sleep > 0))
		usleep((time - check_sleep) * 1000);
	return (0);
}

int	eat_and_check(t_philo *data, int philo_index)
{
	long int	starting_time;
	int			check_sleep;

	starting_time = get_time_elapsed(&data->starting_time);
	check_sleep = 0;
	while (check_sleep + 10 < data->time_to_eat)
	{
		usleep(10000);
		if (check_end_exec(data, philo_index))
			return (-1);
		check_sleep = get_time_elapsed(&data->starting_time) - starting_time;
	}
	if (data->time_to_eat - check_sleep < data->time_to_eat && data->time_to_eat
		- check_sleep > 0)
		usleep((data->time_to_eat - check_sleep) * 1000);
	return (0);
}
