/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 14:03:56 by kipouliq          #+#    #+#             */
/*   Updated: 2024/04/15 17:51:05 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	start_mutexes_bis(t_philo *data)
{
	int	error;

	error = -1;
	if (pthread_mutex_init(data->write_lock, NULL) == -1)
	{
		pthread_mutex_destroy(data->philo_count_lock);
		if (data->nb_of_meals != -1)
		{
			pthread_mutex_destroy(data->nb_of_meals_lock);
			pthread_mutex_destroy(data->f_meals_lock);
		}
		return (-1);
	}
	if (pthread_mutex_init(data->deaths_lock, NULL) == -1)
	{
		pthread_mutex_destroy(data->philo_count_lock);
		pthread_mutex_destroy(data->write_lock);
		if (data->nb_of_meals != -1)
		{
			pthread_mutex_destroy(data->nb_of_meals_lock);
			pthread_mutex_destroy(data->f_meals_lock);
		}
		return (-1);
	}
	return (0);
}

int	start_mutexes(t_philo *data)
{
	if (pthread_mutex_init(data->philo_count_lock, NULL) == -1)
		return (-1);
	if (data->nb_of_meals != -1)
	{
		if (pthread_mutex_init(data->nb_of_meals_lock, NULL) == -1)
		{
			pthread_mutex_destroy(data->philo_count_lock);
			return (-1);
		}
		if (pthread_mutex_init(data->f_meals_lock, NULL) == -1)
		{
			pthread_mutex_destroy(data->philo_count_lock);
			pthread_mutex_destroy(data->nb_of_meals_lock);
			return (-1);
		}
	}
	if (start_mutexes_bis(data) == -1)
		return (-1);
	return (0);
}

int	init_mutexes(t_philo *data)
{
	data->philo_count_lock = malloc(sizeof(t_mutex));
	data->write_lock = malloc(sizeof(t_mutex));
	data->f_meals_lock = malloc(sizeof(t_mutex));
	data->nb_of_meals_lock = malloc(sizeof(t_mutex));
	data->deaths_lock = create_mutex_tab(data->total_philo_nb);
	data->forks = create_mutex_tab(data->total_philo_nb);
	if (!data->philo_count_lock || !data->nb_of_meals_lock
		|| !data->f_meals_lock || !data->write_lock || !data->deaths_lock
		|| !data->forks)
	{
		ft_free_struct(data);
		return (-1);
	}
	if (start_mutexes(data) == -1)
	{
		ft_free_struct(data);
		return (-1);
	}
	return (0);
}

int	init_tabs(t_philo *data)
{
	int	i;

	i = -1;
	data->philo_deaths = malloc(sizeof(int) * (data->total_philo_nb));
	if (!data->philo_deaths)
		return (-1);
	while (++i < data->total_philo_nb)
		data->philo_deaths[i] = 0;
	if (data->nb_of_meals != -1)
	{
		data->philo_f_meals = malloc(sizeof(int) * (data->total_philo_nb));
		if (!data->philo_f_meals)
		{
			free(data->philo_deaths);
			return (-1);
		}
		i = -1;
		while (++i < data->total_philo_nb)
			data->philo_f_meals[i] = 0;
	}
	else
		data->philo_f_meals = NULL;
	if (init_mutexes(data) == -1)
		return (-1);
	return (0);
}

int	init_philo_data(int argc, char **argv, t_philo *data)
{
	data->philo_nb = 0;
	data->total_philo_nb = ft_atoi(argv[0]);
	data->time_to_die = ft_atoi(argv[1]);
	data->time_to_eat = ft_atoi(argv[2]);
	data->time_to_sleep = ft_atoi(argv[3]);
	if (argc == 6)
		data->nb_of_meals = ft_atoi(argv[4]);
	else
		data->nb_of_meals = -1;
	if (is_even(data->total_philo_nb))
		data->time_to_think = data->time_to_eat - data->time_to_sleep - 10;
	else
		data->time_to_think = 2 * data->time_to_eat - data->time_to_sleep - 10;
	if (!check_data(data, argc))
		return (-1);
	gettimeofday(&data->starting_time, NULL);
	return (0);
}
