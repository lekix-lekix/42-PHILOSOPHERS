/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 12:10:57 by kipouliq          #+#    #+#             */
/*   Updated: 2024/04/22 18:30:03 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

int	free_stuff(t_philo *data)
{
	free(data->philos_pids);
	sem_unlink("forks");
	return (0);
}

int	init_semaphores(t_philo *data)
{
	data->locks->forks = sem_open("forks", O_CREAT, 644, data->total_philo_nb);
	if (!data->locks->forks)
		return (-1);
	data->locks->philo_index = sem_open("philo_index", O_CREAT, 644, 1);
	if (!data->locks->philo_index)
		return (-1);
	data->locks->finished_meals = sem_open("finished_meals", O_CREAT, 644, 0);
	if (!data->locks->finished_meals)
		return (-1);
	data->locks->deaths = sem_open("deaths", O_CREAT, 644, 0);
	if (!data->locks->deaths)
		return (-1);
	return (0);
}

int	eat_routine(t_philo *data, long int *last_meal, int philo_index)
{
	sem_wait(data->locks->forks);
	printf("%d has taken a fork\n", philo_index);
	sem_wait(data->locks->forks);
	printf("%d has taken a fork\n", philo_index);
	printf("%d is eating\n", philo_index);
	*last_meal = get_time_elapsed(&data->starting_time);
	usleep(data->time_to_eat * 1000); // need to add checking
	sem_post(data->locks->forks);
	sem_post(data->locks->forks);
    sem_post(data->locks->finished_meals);
	return (0);
}

int	start_philo_routine(t_philo *data, int philo_index)
{
    int         finished_meals;
	long int	last_meal;

    finished_meals = 0;
	while (1)
	{
		eat_routine(data, &last_meal, philo_index);
        usleep(1000);
	}
	return (0);
}

int	init_forking(t_philo *data)
{
	pid_t	*pids;
	int		i;

	i = 0;
	pids = malloc(sizeof(pid_t) * (data->total_philo_nb + 2));
	if (!pids)
		return (-1);
	data->philos_pids = pids;
	gettimeofday(&data->starting_time, NULL);
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
	if (init_semaphores(data) == -1)
		return (-1);
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
	data->locks = malloc(sizeof(t_sem));
	if (!data->locks)
		return (-1);
	return (0);
}

int	monitor_philos_meals(t_philo *data)
{
	int	i;

	i = 0;
	while (i < data->nb_of_meals * data->total_philo_nb)
	{
		sem_wait(data->locks->finished_meals);
		i++;
	}
	return (1);
}

int	monitor_philos_deaths(t_philo *data)
{
	sem_wait(data->locks->deaths);
	return (1);
}

int kill_all_processes(t_philo *data)
{
    int i;

    i = -1;
    while (++i < data->total_philo_nb)
        kill(data->philos_pids[i], SIGKILL);
    return (0);
}

int	main(int argc, char **argv)
{
	t_philo	data;
	pid_t	pid_wait;

	pid_wait = 0;
	sem_unlink("philo_index");
	sem_unlink("forks");
	sem_unlink("finished_meals");
    sem_unlink("deaths");
	if (argc != 5 && argc != 6)
		return (-1);
	argv += 1;
	if (init_philo_data(argc, argv, &data) == -1)
		return (-1);
	if (init_tabs(&data) == -1)
		return (free_stuff(&data));
	if (init_forking(&data) == -1)
	{
		printf("forking failed\n");
		return (-1);
	}
	(void)pid_wait;
	if (monitor_philos_meals(&data))
        kill_all_processes(&data);
	while (1)
	{
		pid_wait = waitpid(-1, NULL, 0);
		if (pid_wait > -1)
			break ;
		else
			usleep(1000);
	}
	return (0);
}
