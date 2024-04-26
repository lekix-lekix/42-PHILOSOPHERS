/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 15:56:51 by kipouliq          #+#    #+#             */
/*   Updated: 2024/04/26 17:05:59 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

int	init_semaphores(t_philo *data)
{
	data->locks->forks = sem_open("forks", O_CREAT, 644, data->total_philo_nb);
	data->locks->philo_index = sem_open("philo_index", O_CREAT, 644, 1);
	data->locks->finished_meals = sem_open("finished_meals", O_CREAT, 644, 0);
	data->locks->deaths = sem_open("deaths", O_CREAT, 644,
			data->total_philo_nb);
	data->locks->write = sem_open("write", O_CREAT, 644, 1);
	if (!data->locks->forks || !data->locks->philo_index
		|| !data->locks->finished_meals || !data->locks->deaths
		|| !data->locks->write)
		return (free_stuff(data), -1);
	return (0);
}

int	init_forking(t_philo *data)
{
	pid_t	pid;
	int		i;

	gettimeofday(&data->starting_time, NULL);
	i = -1;
	while (++i < data->total_philo_nb)
	{
        pid = fork();
		if (pid == -1)
        {
            data->pids_created = i;
			return (-1);
        }
		if (pid == 0)
			start_philo_routine(data, i + 1);
		data->philos_pids[i] = pid;
		usleep(10);
	}
    data->pids_created = i;
	return (0);
}

int	init_philo_data(int argc, char **argv, t_philo *data)
{
	if (!check_args(argv))
		return (-1);
	data->philo_index = 0;
	data->philo_f_meals = 0;
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
	data->locks = malloc(sizeof(t_sem) * 5);
	data->philos_pids = malloc(sizeof(pid_t) * data->total_philo_nb);
	if (!data->locks || !data->philos_pids)
		return (-1);
	return (0);
}
