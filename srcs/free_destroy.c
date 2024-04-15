/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_destroy.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 17:37:31 by kipouliq          #+#    #+#             */
/*   Updated: 2024/04/15 17:40:32 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	ft_destroy_mutexes(t_philo *data)
{
	int	i;

	i = -1;
	if (data->deaths_lock)
		pthread_mutex_destroy(data->deaths_lock);
	if (data->total_philo_nb > 1 && data->f_meals_lock)
		pthread_mutex_destroy(data->f_meals_lock);
	if (data->total_philo_nb > 1 && data->nb_of_meals_lock)
		pthread_mutex_destroy(data->nb_of_meals_lock);
	while (data->forks && ++i < data->total_philo_nb)
		pthread_mutex_destroy(&data->forks[i]);
	if (data->philo_count_lock)
		pthread_mutex_destroy(data->philo_count_lock);
	if (data->write_lock)
		pthread_mutex_destroy(data->write_lock);
	return (0);
}

int	ft_free_struct(t_philo *data)
{
	if (data->deaths_lock)
		free(data->deaths_lock);
	if (data->f_meals_lock)
		free(data->f_meals_lock);
	if (data->nb_of_meals_lock)
		free(data->nb_of_meals_lock);
	if (data->forks)
		free(data->forks);
	if (data->philo_count_lock)
		free(data->philo_count_lock);
	if (data->write_lock)
		free(data->write_lock);
	if (data->philo_deaths)
		free(data->philo_deaths);
	if (data->philo_f_meals)
		free(data->philo_f_meals);
	return (-1);
}

int	free_everything(t_philo *data)
{
	ft_destroy_mutexes(data);   
	ft_free_struct(data);
	return (0);
}
