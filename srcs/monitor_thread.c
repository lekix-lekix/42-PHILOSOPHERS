/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_thread.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 12:21:22 by kipouliq          #+#    #+#             */
/*   Updated: 2024/04/16 13:58:26 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

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

int	fill_death_tab(t_philo *data)
{
	int	i;

	i = -1;
	while (++i < data->total_philo_nb)
	{
		pthread_mutex_lock(&data->deaths_lock[i]);
		data->philo_deaths[i] = 1;
		pthread_mutex_unlock(&data->deaths_lock[i]);
	}
	return (0);
}

int	some_philo_died(t_philo *data, int i)
{
	pthread_mutex_unlock(&data->deaths_lock[i]);
	fill_death_tab(data);
	pthread_mutex_lock(data->write_lock);
	printf("%ld %d died\n", get_time_elapsed(&data->starting_time), i + 1);
	pthread_mutex_unlock(data->write_lock);
	return (0);
}

int	monitor_philos(t_philo *data)
{
	int	i;

	while (1)
	{
		i = -1;
		while (++i < data->total_philo_nb)
		{
			pthread_mutex_lock(&data->deaths_lock[i]);
			if (data->philo_deaths[i])
				return (some_philo_died(data, i));
			pthread_mutex_unlock(&data->deaths_lock[i]);
		}
		if (data->nb_of_meals != -1 && are_all_philos_fed(data))
		{
			fill_death_tab(data);
			return (0);
		}
		usleep(100);
	}
}
