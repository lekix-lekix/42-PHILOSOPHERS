/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 12:10:57 by kipouliq          #+#    #+#             */
/*   Updated: 2024/04/24 18:36:25 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

int close_semaphores(t_sem *locks)
{
    sem_close(locks->forks);
    sem_close(locks->deaths);
    sem_close(locks->finished_meals);
    sem_close(locks->write);
    sem_close(locks->philo_index);
    return (0);
}

int	free_stuff(t_philo *data)
{
	free(data->philos_pids);
    close_semaphores(data->locks);
    free(data->locks);
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
	data->locks->deaths = sem_open("deaths", O_CREAT, 644, data->total_philo_nb);
	if (!data->locks->deaths)
		return (-1);
	data->locks->write = sem_open("write", O_CREAT, 644, 1);
	if (!data->locks->write)
		return (-1);
	return (0);
}

int	death_routine(t_philo *data, int philo_index)
{
	sem_wait(data->locks->write);
	printf("%ld %d died\n", get_time_elapsed(&data->starting_time),
		philo_index);
	sem_post(data->locks->deaths);
	exit(-1);
}

int	check_time_to_die(long int *last_meal, t_timeval starting_time, int ttd)
{
	long int	current_time;

	current_time = get_time_elapsed(&starting_time);
	if (current_time - *last_meal >= ttd)
		return (0);
	return (1);
}

int	usleep_and_check(t_philo *data, long int *last_meal, int time)
{
	long int	starting_time;
	int			check_sleep;

	starting_time = get_time_elapsed(&data->starting_time);
	check_sleep = 0;
	while (get_time_elapsed(&data->starting_time) - starting_time + 10 < time)
	{
		usleep(10000);
		if (!check_time_to_die(last_meal, data->starting_time,
				data->time_to_die))
			return (-1);
		check_sleep = get_time_elapsed(&data->starting_time) - starting_time;
	}
	if (time - check_sleep < time && (time - check_sleep > 0))
		usleep((time - check_sleep) * 1000);
	return (0);
}

int	print_fork(t_philo *data, int philo_index)
{
	// sem_wait(data->locks->write);
	printf("%ld %d has taken a fork\n", get_time_elapsed(&data->starting_time),
		philo_index);
	// sem_post(data->locks->write);
	return (0);
}

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
    // sem_wait(data->locks->write);
	printf("%ld %d is eating\n", get_time_elapsed(&data->starting_time),
		philo_index);
    // sem_post(data->locks->write);
	*last_meal = get_time_elapsed(&data->starting_time);
	if (usleep_and_check(data, last_meal, data->time_to_eat) == -1)
		death_routine(data, philo_index);
	sem_post(data->locks->forks);
	sem_post(data->locks->forks);
	return (0);
}

int	sleep_routine(t_philo *data, long int *last_meal, int philo_index)
{
	// sem_wait(data->locks->write);
	printf("%ld %d is sleeping\n", get_time_elapsed(&data->starting_time),
		philo_index);
	sem_post(data->locks->write);
	if (usleep_and_check(data, last_meal, data->time_to_sleep) == -1)
		death_routine(data, philo_index);
	return (0);
}

int	think_routine(t_philo *data, long int *last_meal, int philo_index)
{
	// sem_wait(data->locks->write);
	printf("%ld %d is thinking\n", get_time_elapsed(&data->starting_time),
		philo_index);
	sem_post(data->locks->write);
	if (usleep_and_check(data, last_meal, data->time_to_think) == -1)
		death_routine(data, philo_index);
	return (0);
}

int	solo_philo_routine(t_philo *data)
{
	sem_wait(data->locks->forks);
	// sem_wait(data->locks->write);
	printf("%ld %d has taken a fork\n", get_time_elapsed(&data->starting_time),
		1);
	usleep(data->time_to_die * 1000);
	// sem_post(data->locks->write);
	sem_post(data->locks->forks);
	death_routine(data, 1);
	return (0);
}

int	start_philo_routine(t_philo *data, int philo_index)
{
	int			finished_meals;
	long int	last_meal;

	finished_meals = 0;
    last_meal = 0;
    sem_wait(data->locks->deaths);
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

int	init_forking(t_philo *data)
{
	pid_t	pid;
	int		i;

	data->philos_pids = malloc(sizeof(pid_t) * (data->total_philo_nb));
	if (!data->philos_pids)
		return (-1);
	gettimeofday(&data->starting_time, NULL);
	i = -1;
	while (++i < data->total_philo_nb)
	{
		pid = fork();
		if (pid == -1)
		{
			free(data->philos_pids);
			return (-1);
		}
		if (pid == 0)
		    start_philo_routine(data, i + 1);
		data->philos_pids[i] = pid;
        usleep(10);
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

int	kill_all_processes(t_philo *data)
{
	int	i;

	i = -1;
	while (++i < data->total_philo_nb)
		kill(data->philos_pids[i], SIGKILL);
    exit(1);
	return (0);
}

int	monitor_philos_meals(t_philo *data)
{
	int	i;

	i = -1;
    // printf("coucou_monitor_meals\n");
	while (++i < data->total_philo_nb)
		sem_wait(data->locks->finished_meals);
    sem_wait(data->locks->write);
    kill_all_processes(data);
	return (1);
}

int	monitor_philos_deaths(t_philo *data)
{
    // printf("coucou_monitor_deaths\n");
	sem_wait(data->locks->deaths);
	kill_all_processes(data);
	return (1);
}

int	run_monitors(t_philo *data, pid_t *monitors)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
		monitor_philos_meals(data);
    monitors[0] = pid;
    pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
		monitor_philos_deaths(data);
    monitors[1] = pid;
    return (0);
	// pid = fork();
	// if (pid == -1)
	// 	return (-1);
	// data->monitors[1] = pid;
	// if (pid == 0)
	// 	monitor_philos_deaths(data);
	// return (0);
}

int unlink_semaphores(void)
{
    sem_unlink("philo_index");
	sem_unlink("forks");
	sem_unlink("finished_meals");
	sem_unlink("deaths");
    sem_unlink("write");
    return (0);
}

int	main(int argc, char **argv)
{
	t_philo	data;
	pid_t	pid_wait;
    pid_t   monitors[2];
    int     i;

	if (argc != 5 && argc != 6)
		return (-1);
	argv += 1;
    unlink_semaphores();
	pid_wait = 0;
	if (init_philo_data(argc, argv, &data) == -1)
		return (-1);
	if (init_tabs(&data) == -1)
		return (free_stuff(&data));
	if (init_forking(&data) == -1)
		return (-1);
    run_monitors(&data, monitors);
	i = 0;
    while (1)
	{
		pid_wait = waitpid(-1, NULL, 0);
        if (pid_wait > -1)
            break;
        usleep(100);
	}
    kill(monitors[0], SIGKILL);
    kill(monitors[1], SIGKILL);
    waitpid(monitors[0], NULL, 0);
    waitpid(monitors[1], NULL, 0);
    free_stuff(&data);
	return (0);
}
