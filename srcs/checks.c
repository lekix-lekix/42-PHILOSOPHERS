/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 14:05:58 by kipouliq          #+#    #+#             */
/*   Updated: 2024/04/12 16:38:58 by kipouliq         ###   ########.fr       */
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

int	usleep_and_check(t_philo *data, long int *last_meal, int time, int philo_index)
{
	int	i;
	int	limit;

	i = 0;
	limit = time / 10;
	while (i < limit)
	{
		usleep(10000);
		if (!check_time_to_die(last_meal, data->starting_time,
				data->time_to_die))
			return (-1); // a changer
		if (check_end_exec(data, philo_index))
			return (-1);
		i++;
	}
	return (0);
}

int	eat_and_check(t_philo *data, int philo_index)
{
	int	i;
	int	limit;

	i = 0;
	limit = data->time_to_eat / 10;
	while (i < limit)
	{
		usleep(10000);
		if (check_end_exec(data, philo_index))
			return (1);
		i++;
	}
	return (0);
}

int	are_all_philos_fed(t_philo *data)
{
	int	i;

	i = -1;
	while (++i < data->total_philo_nb)
	{
		pthread_mutex_lock(data->f_meals_lock);
		if (!data->philo_f_meals[i])
		{
			pthread_mutex_unlock(data->f_meals_lock);
			return (0);
		}
		pthread_mutex_unlock(data->f_meals_lock);
	}
	return (1);
}
