/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines_bis.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 13:51:21 by kipouliq          #+#    #+#             */
/*   Updated: 2024/04/15 12:12:51 by kipouliq         ###   ########.fr       */
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

int	death_routine(t_philo *data, t_mutex *fork_1, t_mutex *fork_2,
		int philo_index)
{
	if (data->total_philo_nb == 1)
	{
		pthread_mutex_lock(data->deaths_lock);
		data->philo_deaths[0] = 1;
		pthread_mutex_unlock(data->deaths_lock);
		return (0);
	}
	if (fork_1)
		pthread_mutex_unlock(fork_1);
	if (fork_2)
		pthread_mutex_unlock(fork_2);
	pthread_mutex_lock(&data->deaths_lock[philo_index - 1]);
	data->philo_deaths[philo_index - 1] = 1;
	pthread_mutex_unlock(&data->deaths_lock[philo_index - 1]);
	return (-1);
}

int	sleep_routine(t_philo *data, long int *last_meal, int philo_index)
{
	pthread_mutex_lock(data->write_lock);
	if (check_end_exec(data, philo_index))
	{
		pthread_mutex_unlock(data->write_lock);
		return (0);
	}
	printf("%ld %d is sleeping\n", get_time_elapsed(&data->starting_time),
		philo_index);
	pthread_mutex_unlock(data->write_lock);
	usleep_and_check(data, last_meal, data->time_to_sleep, philo_index);
	return (0);
}

int	think_routine(t_philo *data, long int *last_meal, int philo_index)
{
	pthread_mutex_lock(data->write_lock);
	if (check_end_exec(data, philo_index))
	{
		pthread_mutex_unlock(data->write_lock);
		return (-1);
	}
	printf("%ld %d is thinking\n", get_time_elapsed(&data->starting_time),
		philo_index);
	pthread_mutex_unlock(data->write_lock);
	if (data->time_to_think <= 0)
		usleep(1000);
	else
	{
		if (usleep_and_check(data, last_meal, data->time_to_think,
				philo_index) == -1)
			return (death_routine(data, NULL, NULL, philo_index));
	}
	return (0);
}

int	start_meal(t_mutex_pair *forks, t_philo *data, long int *last_meal,
		int philo_index)
{
	pthread_mutex_lock(data->write_lock);
	if (check_end_exec(data, philo_index))
	{
		pthread_mutex_unlock(data->write_lock);
		pthread_mutex_unlock(forks->fork_1);
		pthread_mutex_unlock(forks->fork_2);
		return (-1);
	}
	printf("%ld %d is eating\n", get_time_elapsed(&data->starting_time),
		philo_index);
	pthread_mutex_unlock(data->write_lock);
	*last_meal = get_time_elapsed(&data->starting_time);
	eat_and_check(data, philo_index);
	pthread_mutex_unlock(forks->fork_1);
	pthread_mutex_unlock(forks->fork_2);
	return (0);
}
