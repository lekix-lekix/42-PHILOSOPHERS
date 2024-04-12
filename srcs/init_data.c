/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 14:03:56 by kipouliq          #+#    #+#             */
/*   Updated: 2024/04/12 18:25:00 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	start_mutexes(t_philo *data)
{
	if (pthread_mutex_init(data->philo_count_lock, NULL) == -1)
		return (-1);
	if (data->nb_of_meals)
	{
		if (pthread_mutex_init(data->nb_of_meals_lock, NULL) == -1)
			return (-1);
		if (pthread_mutex_init(data->f_meals_lock, NULL) == -1)
			return (-1);
	}
	if (pthread_mutex_init(data->write_lock, NULL) == -1)
		return (-1);
	if (pthread_mutex_init(data->deaths_lock, NULL) == -1)
		return (-1);
	return (0);
}

int	init_mutexes(t_philo *data)
{
	t_mutex	*philo_nb;
	t_mutex	*meals_nb;
	t_mutex	*f_eating;
	t_mutex	*deaths;
	t_mutex	*write;

	philo_nb = malloc(sizeof(t_mutex));
	meals_nb = malloc(sizeof(t_mutex));
	f_eating = malloc(sizeof(t_mutex));
	deaths = create_mutex_tab(data->total_philo_nb);
	write = malloc(sizeof(t_mutex));
	if (!philo_nb || !meals_nb || !f_eating || !write || !deaths)
		return (-1);
	data->philo_count_lock = philo_nb;
	data->nb_of_meals_lock = meals_nb;
	data->f_meals_lock = f_eating;
	data->deaths_lock = deaths;
	data->write_lock = write;
	if (start_mutexes(data) == -1)
		return (-1);
	return (0);
}

int	init_philo_data(int argc, char **argv, t_philo *data)
{
	data->philo_nb = 0;
	data->end_exec = 0;
	data->total_philo_nb = ft_atoi(argv[0]);
	data->time_to_die = ft_atoi(argv[1]);
	data->time_to_eat = ft_atoi(argv[2]);
	data->time_to_sleep = ft_atoi(argv[3]);
	if (argc == 6)
		data->nb_of_meals = ft_atoi(argv[4]);
	else
		data->nb_of_meals = -1;
	data->forks = create_mutex_tab(data->total_philo_nb);
    if (!data->forks)
        return (-1);
	if (is_even(data->total_philo_nb))
		data->time_to_think = data->time_to_eat - data->time_to_sleep - 10;
	else
		data->time_to_think = 2 * data->time_to_eat - data->time_to_sleep - 10;
	if (init_mutexes(data) == -1)
		return (-1);
	gettimeofday(&data->starting_time, NULL);
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
		if (!data->philo_deaths)
			return (-1);
		i = -1;
		while (++i < data->total_philo_nb)
			data->philo_f_meals[i] = 0;
	}
	else
		data->philo_f_meals = NULL;
	return (0);
}
