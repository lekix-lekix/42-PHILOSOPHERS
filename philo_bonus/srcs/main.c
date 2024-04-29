/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 12:10:57 by kipouliq          #+#    #+#             */
/*   Updated: 2024/04/29 15:32:17 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

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

int	init_all(t_philo *data, int argc, char **argv)
{
	if (init_philo_data(argc, argv, data) == -1)
		return (-1);
	if (init_semaphores(data) == -1)
		return (-1);
	if (init_forking(data) == -1)
	{
		kill_all_processes(data);
		free_stuff(data);
		exit(0);
	}
	return (0);
}

int	wait_kill(t_philo *data)
{
	pid_t	monitors[2];
	int		pid_wait;

	run_monitors(data, monitors);
	while (1)
	{
		pid_wait = waitpid(-1, NULL, 0);
		if (pid_wait > -1)
			break ;
		usleep(100);
	}
	sem_wait(data->locks->killing);
	kill(monitors[0], SIGKILL);
	kill(monitors[1], SIGKILL);
	return (0);
}

int	main(int argc, char **argv)
{
	t_philo	data;

	if (argc != 5 && argc != 6)
		return (-1);
	argv += 1;
	unlink_semaphores();
	if (init_all(&data, argc, argv) == -1)
		return (-1);
	wait_kill(&data);
	free_stuff(&data);
	return (0);
}
