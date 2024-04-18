/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 12:10:57 by kipouliq          #+#    #+#             */
/*   Updated: 2024/04/18 17:31:40 by kipouliq         ###   ########.fr       */
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

// int	eat_routine(t_philo *data, long int *last_meal, int philo_index)
// {
//     sem_wait()
// 	if (!check_time_to_die(last_meal, data->starting_time, data->time_to_die))
// 		return (death_routine(data, forks.fork_1, forks.fork_2, philo_index));
// 	return (start_meal(&forks, data, last_meal, philo_index));
// }

int	start_philo_routine(t_philo *data, int pid, int philo_index)
{
	// int	i;

	// long int    last_meal;
	sleep(1);
	printf("philo idx = %d\n", philo_index);
	printf("hi im process %d\n", pid);
	printf("philo total = %d\n", data->total_philo_nb);
	// eat_routine(data, philo_index);
	free_stuff(data);
	printf("philo %d is dead\n", philo_index);
	exit(1);
}

int	init_forking(t_philo *data)
{
	pid_t	*pids;
	int		i;

	i = 0;
	pids = malloc(sizeof(pid_t) * data->total_philo_nb);
	if (!pids)
		return (-1);
	data->philos_pids = pids;
	gettimeofday(&data->starting_time, NULL);
	while (i < data->total_philo_nb)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
            printf("fork problem\n");
			free(pids);
			return (-1);
		}
		if (pids[i] != 0)
			start_philo_routine(data, pids[i], i);
		i++;
        // pid_wait = waitpid(pids[i], &status, WNOHANG);
        // if (pid_wait == -1)
        // {
        //     printf("GROSSE ERRURR\n");
        //     perror("waitpid");
        // }
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
	return (0);
}

// int	wait_pids(void)
// {
// 	int		status;
// 	pid_t	pid;

// 	// Wait for any child process to terminate
// 	if (pid > 0)
// 	{
// 		// A child process has terminated
// 		printf("Child process with PID %d has finished\n", pid);
// 		printf("status = %d\n", status);
//         return (1);
// 	}
// 	else if (pid == -1)
// 	{
// 		// Error occurred
// 		perror("waitpid error");
// 	}
// 	else
// 	{
// 		// No child process has terminated yet
// 		printf("No child process has finished yet\n");
// 	}
// 	return (0);
// }

int	main(int argc, char **argv)
{
	t_philo	data;
    int status;
    pid_t pid_wait = 0;

	sem_unlink("philo_index");
	sem_unlink("forks");
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
    // wait_pids();
	// while (1)
    // {
    //     if (wait_pids())
    //         break;
    // }

        // Wait for the first child process to finish
    // while (pid_wait == 0)
    // {
    //     pid_wait = waitpid(-1, &status, WNOHANG);
    //     if (pid_wait > 0) {
    //         printf("Child process with PID %d has finished\n", pid_wait);
    //         printf("status = %d\n", status);
    //         break; // Exit the loop once a child process finishes
    //     } else if (pid_wait == -1) {
    //         perror("waitpid error");
    //         break; // Exit the loop if an error occurs
    //     } else {
    //         usleep(100000); // Sleep for 100ms before checking again
    //     }
    // }
    (void) status;
    (void) pid_wait;
    while (1)
    {
        pid_wait = waitpid(-1, NULL, 0);
        // printf("pid_wait %d\n", pid_wait);
        if(pid_wait > -1)
            break;
        else
            usleep(1000);
    }
    // while (waitpid(-1, NULL, 0) > -1);
    // {
    //     pid_wait = waitpid(-1, &status, WNOHANG);
    //     if (pid_wait > 0) {
    //         printf("Child process with PID %d has finished\n", pid_wait);
    //         printf("status = %d\n", status);
    //         break; // Exit the loop once a child process finishes
    //     } else if (pid_wait == -1) {
    //         perror("waitpid error");
    //         break; // Exit the loop if an error occurs
    //     } else {
    //         usleep(100000); // Sleep for 100ms before checking again
    //     }
    // }
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
