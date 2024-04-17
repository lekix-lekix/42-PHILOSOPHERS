/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 12:10:57 by kipouliq          #+#    #+#             */
/*   Updated: 2024/04/17 17:54:40 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

int	free_stuff(t_philo *data)
{
	sem_close(data->sem_forks);
	free(data->philos_pids);
	free(data->philo_deaths);
	free(data->philo_f_meals);
	sem_unlink("forks");
    sem_unlink("philo_index");
	return (0);
}

int	init_semaphores(t_philo *data)
{
	int	value;

	data->sem_forks = sem_open("forks", O_CREAT, 644, data->total_philo_nb);
	if (!data->sem_forks)
    {
        printf("ca a bug ici\n");
		return (-1);
    }
	data->sem_philo_index = sem_open("philo_index", O_CREAT, 644, 1);
	if (!data->sem_philo_index)
		return (-1);
	sem_getvalue(data->sem_forks, &value);
	printf("sem value = %d\n", value);
	return (0);
}

int	start_philo_routine(t_philo *data, int pid)
{
	int	philo_idx;

    printf("couoccc\n");
	sem_wait(data->sem_philo_index);
	philo_idx = data->philo_index;
	data->philo_index += 1;
	sem_post(data->sem_philo_index);
	printf("philo idx = %d\n", philo_idx);
	printf("hi im process %d\n", pid);
	printf("philo total = %d\n", data->total_philo_nb);
	free_stuff(data);
	exit(1);
}

int	init_forking(t_philo *data)
{
	int	*pids;
	int	i;

	i = 0;
	pids = malloc(sizeof(pid_t) * data->total_philo_nb);
	if (!pids)
		return (-1);
	data->philos_pids = pids;
	while (i < data->total_philo_nb)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			free(pids);
			return (-1);
		}
		if (pids[i] != 0)
			start_philo_routine(data, pids[i]);
		i++;
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
	if (init_semaphores(data) == -1)
		return (-1);
	return (0);
}

int	init_philo_data(int argc, char **argv, t_philo *data)
{
	if (!check_args(argv))
		return (-1);
	data->philo_index = 0;
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

int	wait_all_pids(int *pids, int count)
{
	int	status;
	int	i;

	i = 0;
	while (i < count)
	{
		waitpid(pids[i], &status, WUNTRACED);
		i++;
	}
	return (status);
}

int	main(int argc, char **argv)
{
	t_philo	data;

    sem_unlink("philo_index");
	if (argc != 5 && argc != 6)
		return (-1);
	argv += 1;
	if (init_philo_data(argc, argv, &data) == -1)
		return (-1);
	if (init_tabs(&data) == -1)
		return (free_stuff(&data));
	if (init_forking(&data) == -1)
		return (-1);
	wait_all_pids(data.philo_deaths, data.total_philo_nb);
	free_stuff(&data);
	// philos = create_philos(&data);
	// if (!philos)
	// {
	// 	fill_death_tab(&data);
	// 	free_everything(&data);
	// 	free(philos);
	// 	return (-1);
	// }
	// monitor_philos(&data);
	// wait_philos(philos, data.total_philo_nb);
	// free_everything(&data);
	// free(philos);
	return (0);
}
