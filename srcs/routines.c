/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 13:44:43 by kipouliq          #+#    #+#             */
/*   Updated: 2024/04/15 18:26:37 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	print_fork(t_philo *data, int philo_index)
{
	pthread_mutex_lock(data->write_lock);
	if (check_end_exec(data, philo_index))
	{
		pthread_mutex_unlock(data->write_lock);
		return (0);
	}
	printf("%ld %d has taken a fork\n", get_time_elapsed(&data->starting_time),
		philo_index);
	pthread_mutex_unlock(data->write_lock);
	return (0);
}

int	lock_forks(t_mutex *fork_1, t_mutex *fork_2, t_philo *data, int philo_index)
{
	pthread_mutex_lock(fork_1);
	if (check_end_exec(data, philo_index))
	{
		pthread_mutex_unlock(fork_1);
		return (-1);
	}
	print_fork(data, philo_index);
	pthread_mutex_lock(fork_2);
	if (check_end_exec(data, philo_index))
	{
		pthread_mutex_unlock(fork_1);
		pthread_mutex_unlock(fork_2);
		return (-1);
	}
	print_fork(data, philo_index);
	return (0);
}

int	eat_routine(t_philo *data, long int *last_meal, int philo_index)
{
	t_mutex_pair	forks;

	forks.fork_1 = &data->forks[philo_index - 1];
	if (philo_index == data->total_philo_nb)
		forks.fork_2 = &data->forks[0];
	else
		forks.fork_2 = &data->forks[philo_index];
	if (is_even(philo_index))
	{
		if (lock_forks(forks.fork_1, forks.fork_2, data, philo_index) == -1)
			return (-1);
	}
	else
	{
		if (lock_forks(forks.fork_2, forks.fork_1, data, philo_index) == -1)
			return (-1);
	}
	if (!check_time_to_die(last_meal, data->starting_time, data->time_to_die))
		return (death_routine(data, forks.fork_1, forks.fork_2, philo_index));
	return (start_meal(&forks, data, last_meal, philo_index));
}

int	start_philo_routine(t_philo *data, t_routine_data *infos)
{
	while (1)
	{
		usleep(100);
		if (check_end_exec(data, infos->philo_index))
			return (0);
		if (data->total_philo_nb == 1)
			return (only_one_philo_routine(data));
		if (infos->meals_finished == infos->max_meals)
		{
			pthread_mutex_lock(data->f_meals_lock);
			data->philo_f_meals[infos->philo_index - 1] = 1;
			pthread_mutex_unlock(data->f_meals_lock);
		}
		if (eat_routine(data, &infos->last_meal, infos->philo_index) == -1)
			return (-1);
		if (sleep_routine(data, &infos->last_meal, infos->philo_index) == -1)
			return (-1);
		if (think_routine(data, &infos->last_meal, infos->philo_index) == -1)
			return (-1);
		infos->meals_finished += 1;
	}
	return (0);
}

int	philo_routine(t_philo *data)
{
	t_routine_data	infos;

	infos.meals_finished = 0;
	infos.last_meal = 0;
	pthread_mutex_lock(data->nb_of_meals_lock);
	infos.max_meals = data->nb_of_meals;
	pthread_mutex_unlock(data->nb_of_meals_lock);
	pthread_mutex_lock(data->philo_count_lock);
	data->philo_nb += 1;
	infos.philo_index = data->philo_nb;
	pthread_mutex_unlock(data->philo_count_lock);
	if (check_end_exec(data, infos.philo_index))
		return (0);
	if (is_even(infos.philo_index))
		usleep((data->time_to_eat * 1000) - 100);
	if (start_philo_routine(data, &infos) == -1)
		return (-1);
	return (0);
}
