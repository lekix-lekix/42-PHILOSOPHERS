/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 12:21:14 by kipouliq          #+#    #+#             */
/*   Updated: 2024/03/28 18:11:04 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

// ./philo 4 100 100 100 5

long int    get_time_elapsed(t_timeval *starting_time)
{
	t_timeval	current_time;
	long int	time_elapsed;

	gettimeofday(&current_time, NULL);
    time_elapsed = ((current_time.tv_sec - starting_time->tv_sec) * 1000) + ((current_time.tv_usec - starting_time->tv_usec) / 1000);
	return (time_elapsed);
}

int	ft_atoi(const char *nptr)
{
	int	i;
	int	neg;
	int	nb;

	nb = 0;
	i = 0;
	neg = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			neg = 1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		nb = nb * 10 + (nptr[i] - '0');
		i++;
	}
	if (neg)
		nb = -nb;
	return (nb);
}

int	print_fork(t_philo *data, int philo_index)
{
	pthread_mutex_lock(data->write_lock);
	printf("%ld %d has taken a fork\n", get_time_elapsed(&data->starting_time), philo_index);
	pthread_mutex_unlock(data->write_lock);
	return (0);
}

int	sleep_routine(t_philo *data, int philo_index)
{
	pthread_mutex_lock(data->write_lock);
    printf("%ld %d is sleeping\n", get_time_elapsed(&data->starting_time), philo_index);
	pthread_mutex_unlock(data->write_lock);
	usleep(data->time_to_sleep * 1000);
	return (0);
}

int	lock_forks(t_philo *data, int philo_index)
{
	pthread_mutex_t	fork_2;
    pthread_mutex_t fork_1; // pas necessaire
	
    fork_1 = data->forks[philo_index];
	if (philo_index == data->total_philo_nb - 1)
		fork_2 = data->forks[0];
	else
		fork_2 = data->forks[philo_index + 1];
	if (philo_index % 2 == 0)
	{
		pthread_mutex_lock(&fork_1);
		print_fork(data, philo_index);
		pthread_mutex_lock(&fork_2);
		print_fork(data, philo_index);
	}
	else
	{
		pthread_mutex_lock(&fork_2);
		print_fork(data, philo_index);
		pthread_mutex_lock(&fork_1);
		print_fork(data, philo_index);
	}
	pthread_mutex_lock(data->write_lock);
	printf("%ld %d is eating\n", get_time_elapsed(&data->starting_time), philo_index);
	pthread_mutex_unlock(data->write_lock);
	usleep(data->time_to_eat);
	pthread_mutex_unlock(&fork_1);
	pthread_mutex_unlock(&fork_2);
	return (0);
}

int	eat_routine(t_philo *data, int philo_index)
{
	lock_forks(data, philo_index);
	return (0);
}

int	think_routine(t_philo *data, int philo_index)
{
	pthread_mutex_lock(data->write_lock);
	printf("%ld %d is thinking\n", get_time_elapsed(&data->starting_time), philo_index);
	pthread_mutex_unlock(data->write_lock);
	usleep(data->time_to_die * 1000);
	return (0);
}

void	philo_routine(t_philo *data)
{
	int	philo_index;
	int	meals_finished;

	meals_finished = 0;
	pthread_mutex_lock(data->philo_count_lock);
	philo_index = data->philo_nb;
	pthread_mutex_lock(data->write_lock);
	printf("philo nb %d is created\n", philo_index);
	pthread_mutex_unlock(data->write_lock);
	data->philo_nb += 1;
	pthread_mutex_unlock(data->philo_count_lock);
	while (meals_finished != data->nb_of_meals)
	{
		eat_routine(data, philo_index);
		sleep_routine(data, philo_index);
		think_routine(data, philo_index);
		meals_finished += 1;
	}
	// printf("meals finished = %d\n", meals_finished);
	// while (meals_finished < data->nb_of_meals)
	// {,
	// }
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

pthread_mutex_t	*create_forks(int forks_nb)
{
	pthread_mutex_t	*forks;
	int				i;
	int				error;

	forks = malloc(sizeof(pthread_mutex_t) * (forks_nb));
	if (!forks)
		return (NULL);
	i = -1;
	while (++i < forks_nb)
	{
		error = pthread_mutex_init(&forks[i], NULL);
		if (error)
		{
			printf("mutex error : %d\n", error);
			return (NULL);
		}
	}
	return (forks);
}

int	wait_philos(pthread_t *philos_tab, int philos_nb)
{
	int	i;
	int	error;

	i = -1;
	while (++i < philos_nb)
	{
		error = pthread_join(philos_tab[i], NULL);
		if (error)
			return (error);
	}
	return (0);
}

int	init_philo_data(char **argv, pthread_mutex_t *forks_tab, t_philo *data)
{
	pthread_mutex_t	*philo_nb_mutex;
	pthread_mutex_t	*write;

	philo_nb_mutex = malloc(sizeof(pthread_mutex_t));
	if (!philo_nb_mutex)
		return (-1);
	if (pthread_mutex_init(philo_nb_mutex, NULL) == -1)
		return (-1);
	write = malloc(sizeof(pthread_mutex_t));
	if (!philo_nb_mutex)
		return (-1);
	if (pthread_mutex_init(write, NULL) == -1)
		return (-1);
	data->philo_count_lock = philo_nb_mutex;
	data->write_lock = write;
	data->philo_nb = 0;
	data->forks = forks_tab;
	data->total_philo_nb = ft_atoi(argv[0]);
	data->time_to_die = ft_atoi(argv[1]);
	data->time_to_eat = ft_atoi(argv[2]);
	data->time_to_sleep = ft_atoi(argv[3]);
	data->nb_of_meals = ft_atoi(argv[4]);
	if (data->total_philo_nb % 2 == 0)
		data->time_to_think = data->time_to_eat - data->time_to_sleep;
	else
		data->time_to_die = data->time_to_eat + data->time_to_sleep;
	return (0);
}

int	main(int argc, char **argv)
{
	pthread_t		*philos;
	pthread_mutex_t	*forks;
	t_philo			data;
	int				philos_nb;

	if (argc != 6)
		return (-1);
    sleep(1);
	argv += 1;
	philos_nb = ft_atoi(argv[0]);
	forks = create_forks(philos_nb);
	if (!forks)
		return (-1);
	init_philo_data(argv, forks, &data);
    gettimeofday(&data.starting_time, NULL);
	philos = create_philos(philos_nb, &data);
	if (!philos)
		return (-1);
	printf("pthread_join = %d\n", wait_philos(philos, philos_nb));
	return (0);
}
