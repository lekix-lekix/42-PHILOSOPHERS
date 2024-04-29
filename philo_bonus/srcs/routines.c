/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 16:28:18 by kipouliq          #+#    #+#             */
/*   Updated: 2024/04/29 14:54:55 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

int	eat_routine(t_philo *data, long int *last_meal, int philo_index)
{
	sem_wait(data->locks->forks);
	if (!check_time_to_die(last_meal, data->starting_time, data->time_to_die))
		return (death_routine(data, philo_index));
	print_fork(data, philo_index);
	sem_wait(data->locks->forks);
	if (!check_time_to_die(last_meal, data->starting_time, data->time_to_die))
		return (death_routine(data, philo_index));
	print_fork(data, philo_index);
	sem_wait(data->locks->write);
	printf("%ld %d is eating\n", get_time_elapsed(&data->starting_time),
		philo_index);
	sem_post(data->locks->write);
	*last_meal = get_time_elapsed(&data->starting_time);
	usleep(data->time_to_eat * 1000);
	sem_post(data->locks->forks);
	sem_post(data->locks->forks);
	return (0);
}

int	sleep_routine(t_philo *data, long int *last_meal, int philo_index)
{
	sem_wait(data->locks->write);
	printf("%ld %d is sleeping\n", get_time_elapsed(&data->starting_time),
		philo_index);
	sem_post(data->locks->write);
	if (usleep_and_check(data, last_meal, data->time_to_sleep) == -1)
		death_routine(data, philo_index);
	return (0);
}

int	think_routine(t_philo *data, long int *last_meal, int philo_index)
{
	sem_wait(data->locks->write);
	printf("%ld %d is thinking\n", get_time_elapsed(&data->starting_time),
		philo_index);
	sem_post(data->locks->write);
	if (usleep_and_check(data, last_meal, data->time_to_think) == -1)
		death_routine(data, philo_index);
	return (0);
}

int	start_philo_routine(t_philo *data, int philo_index)
{
	int			finished_meals;
	long int	last_meal;

	finished_meals = 0;
	last_meal = 0;
	sem_wait(data->locks->deaths);
	sem_wait(data->locks->killing);
	sem_wait(data->locks->finished_meals);
	if (data->total_philo_nb == 1)
		return (solo_philo_routine(data));
	if (is_even(philo_index))
		usleep((data->time_to_eat * 1000) - 100);
	while (1)
	{
		eat_routine(data, &last_meal, philo_index);
		finished_meals += 1;
		if (finished_meals == data->nb_of_meals)
			sem_post(data->locks->finished_meals);
		sleep_routine(data, &last_meal, philo_index);
		think_routine(data, &last_meal, philo_index);
	}
	return (0);
}
