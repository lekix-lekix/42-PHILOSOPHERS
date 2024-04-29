/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitors.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 16:25:00 by kipouliq          #+#    #+#             */
/*   Updated: 2024/04/29 15:32:24 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

int	kill_all_processes(t_philo *data)
{
	int	i;

	i = -1;
	while (++i < data->pids_created)
		kill(data->philos_pids[i], SIGKILL);
	sem_post(data->locks->killing);
	return (0);
}

int	monitor_philos_meals(t_philo *data)
{
	int	i;

	i = -1;
	while (++i < data->total_philo_nb)
		sem_wait(data->locks->finished_meals);
	kill_all_processes(data);
	usleep(100);
	free_stuff(data);
	exit(0);
	return (1);
}

int	monitor_philos_deaths(t_philo *data)
{
	sem_wait(data->locks->deaths);
	kill_all_processes(data);
	usleep(100);
	free_stuff(data);
	exit(0);
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
}
