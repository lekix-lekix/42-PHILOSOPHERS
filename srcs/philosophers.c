/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 12:21:14 by kipouliq          #+#    #+#             */
/*   Updated: 2024/04/12 18:25:11 by kipouliq         ###   ########.fr       */
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

pthread_t	*create_philos(int philos_nb, t_philo *data)
{
	pthread_t	*philos_tab;
	int			i;
	int			error;

	philos_tab = malloc(sizeof(pthread_t) * (philos_nb));
	if (!philos_tab)
		return (NULL);
	i = -1;
	while (++i < philos_nb)
	{
		error = pthread_create(&philos_tab[i], NULL, (void *)philo_routine,
				data);
		if (error)
		{
			printf("error creating pthread\n");
			return (NULL);
		}
	}
	return (philos_tab);
}

t_mutex	*create_mutex_tab(int mtx_nb)
{
	t_mutex	*mtx_tab;
	int		i;
	int		error;

	mtx_tab = malloc(sizeof(t_mutex) * (mtx_nb));
	if (!mtx_tab)
		return (NULL);
	i = -1;
	while (++i < mtx_nb)
	{
		error = pthread_mutex_init(&mtx_tab[i], NULL);
		if (error)
		{
			printf("mutex error : %d\n", error);
			return (NULL);
		}
	}
	return (mtx_tab);
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
				some_philo_died(data, i);
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

int	check_data(t_philo *data)
{
	if (data->total_philo_nb <= 0 || data->time_to_die < 0
		|| data->time_to_eat < 0 || data->time_to_sleep < 0
		|| data->time_to_think < 0)
		return (0);
    return (1);
}

int	main(int argc, char **argv)
{
	pthread_t	*philos;
	t_mutex		*forks;
	t_philo		data;
	int			philos_nb;

	if (argc != 5 && argc != 6)
		return (-1);
	argv += 1;
	if (init_philo_data(argc, argv, forks, &data) == -1)
		return (-1);
	init_tabs(&data);
	if (!check_data(&data))
		return (-1);
	philos = create_philos(philos_nb, &data);
	if (!philos)
		return (-1);
	monitor_philos(&data);
	wait_philos(philos, data.total_philo_nb);
	free_everything(&data);
	free(philos);
	return (0);
}
