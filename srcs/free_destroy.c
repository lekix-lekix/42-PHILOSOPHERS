/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_destroy.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 17:37:31 by kipouliq          #+#    #+#             */
/*   Updated: 2024/04/12 17:37:56 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	ft_destroy_mutexes(t_philo *data)
{
	int	i;

	i = -1;
	while (++i < data->total_philo_nb)
		pthread_mutex_destroy(&data->forks[i]);
	pthread_mutex_destroy(data->philo_count_lock);
	free(data->philo_count_lock);
	pthread_mutex_destroy(data->write_lock);
	free(data->write_lock);
	pthread_mutex_destroy(data->deaths_lock);
	free(data->deaths_lock);
	pthread_mutex_destroy(data->f_meals_lock);
	free(data->f_meals_lock);
	pthread_mutex_destroy(data->nb_of_meals_lock);
	free(data->nb_of_meals_lock);
	return (0);
}

int	free_everything(t_philo *data)
{
	ft_destroy_mutexes(data);
	free(data->forks);
	free(data->philo_deaths);
	if (data->philo_f_meals)
		free(data->philo_f_meals);
	return (0);
}
