/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 12:21:14 by kipouliq          #+#    #+#             */
/*   Updated: 2024/03/29 17:44:50 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

// ./philo 4 100 100 100 5

int	is_even(int nb)
{
	return (nb % 2 == 0);
}

long int	get_time_elapsed(t_timeval *starting_time)
{
	t_timeval	current_time;
	long int	time_elapsed;

	gettimeofday(&current_time, NULL);
	time_elapsed = ((current_time.tv_sec - starting_time->tv_sec) * 1000)
		+ ((current_time.tv_usec - starting_time->tv_usec) * 0.001);
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

int	check_time_to_die(long int *last_meal, t_timeval starting_time, int ttd)
{
	long int	current_time;

	current_time = get_time_elapsed(&starting_time);
	if (current_time - *last_meal >= ttd)
		return (0);
	return (1);
}

int usleep_and_check(t_philo *data, long int *last_meal, int time)
{
    int i;
    int limit;

    i = 0;
    limit = time / 10;
    while (i < limit)
    {
        usleep(10000);
        if (!check_time_to_die(last_meal, data->starting_time, data->time_to_die))
            return (-1);
        i++;
    }
    return (0);
}

int	print_fork(t_philo *data, int philo_index)
{
	pthread_mutex_lock(data->write_lock);
	printf("%ld %d has taken a fork\n", get_time_elapsed(&data->starting_time),
		philo_index);
	pthread_mutex_unlock(data->write_lock);
	return (0);
}

int	sleep_routine(t_philo *data, long int *last_meal, int philo_index)
{
	pthread_mutex_lock(data->write_lock);
	printf("%ld %d is sleeping\n", get_time_elapsed(&data->starting_time),
		philo_index);
	pthread_mutex_unlock(data->write_lock);
	usleep_and_check(data, last_meal, data->time_to_sleep);
	return (0);
}

int	think_routine(t_philo *data, long int *last_meal, int philo_index)
{
	pthread_mutex_lock(data->write_lock);
	printf("%ld %d is thinking\n", get_time_elapsed(&data->starting_time),
		philo_index);
	pthread_mutex_unlock(data->write_lock);
	usleep_and_check(data, last_meal, data->time_to_think);
	return (0);
}

int	lock_forks(t_mutex *fork_1, t_mutex *fork_2, t_philo *data, int philo_index)
{
	pthread_mutex_lock(fork_1);
	print_fork(data, philo_index);
	pthread_mutex_lock(fork_2);
	print_fork(data, philo_index);
	return (0);
}

int	death_routine(t_philo *data, t_mutex *fork_1, t_mutex *fork_2,
		int philo_index)
{
	pthread_mutex_unlock(fork_1);
	pthread_mutex_unlock(fork_2);
	pthread_mutex_lock(data->write_lock);
	printf("%ld %d died\n", get_time_elapsed(&data->starting_time),
		philo_index);
	pthread_mutex_unlock(data->write_lock);
	data->philos_deaths[philo_index] = 1;
	return (-1);
}

int	eat_routine(t_philo *data, long int *last_meal, int philo_index)
{
	t_mutex	*fork_1;
	t_mutex	*fork_2;

	fork_1 = &data->forks[philo_index];
	if (philo_index == data->total_philo_nb - 1)
		fork_2 = &data->forks[0];
	else
		fork_2 = &data->forks[philo_index + 1];
	if (is_even(philo_index))
		lock_forks(fork_1, fork_2, data, philo_index);
	else
		lock_forks(fork_2, fork_1, data, philo_index);
	if (!check_time_to_die(last_meal, data->starting_time, data->time_to_die))
		return (death_routine(data, fork_1, fork_2, philo_index));
	pthread_mutex_lock(data->write_lock);
	printf("%ld %d is eating\n", get_time_elapsed(&data->starting_time),
		philo_index);
	pthread_mutex_unlock(data->write_lock);
	*last_meal = get_time_elapsed(&data->starting_time);
    usleep_and_check(data, last_meal, data->time_to_eat);
	pthread_mutex_unlock(fork_1);
	pthread_mutex_unlock(fork_2);
	return (0);
}

void	philo_routine(t_philo *data)
{
	int			philo_index;
	int			meals_finished;
	long int	last_meal;

	meals_finished = 0;
	last_meal = 0;
	pthread_mutex_lock(data->philo_count_lock);
	philo_index = data->philo_nb;
	pthread_mutex_lock(data->write_lock);
	pthread_mutex_unlock(data->write_lock);
	data->philo_nb += 1;
	pthread_mutex_unlock(data->philo_count_lock);
	while (meals_finished != data->nb_of_meals)
	{
		if (eat_routine(data, &last_meal, philo_index) == -1)
			return ;
		if (sleep_routine(data, &last_meal, philo_index) == -1)
            return ;
		if (think_routine(data, &last_meal, philo_index) == -1)
            return ;
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

t_mutex	*create_forks(int forks_nb)
{
	t_mutex	*forks;
	int		i;
	int		error;

	forks = malloc(sizeof(t_mutex) * (forks_nb));
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

int	init_philo_data(char **argv, t_mutex *forks_tab, t_philo *data)
{
	t_mutex	*philo_nb_mutex;
	t_mutex	*write;
	int		i;

	i = -1;
	philo_nb_mutex = malloc(sizeof(t_mutex));
	if (!philo_nb_mutex)
		return (-1);
	if (pthread_mutex_init(philo_nb_mutex, NULL) == -1)
		return (-1);
	write = malloc(sizeof(t_mutex));
	if (!philo_nb_mutex)
		return (-1);
	if (pthread_mutex_init(write, NULL) == -1)
		return (-1);
	while (++i < 5)
		data->philos_deaths[i] = 0;
	data->philo_count_lock = philo_nb_mutex;
	data->write_lock = write;
	data->philo_nb = 0;
	data->forks = forks_tab;
	data->total_philo_nb = ft_atoi(argv[0]);
	data->time_to_die = ft_atoi(argv[1]);
	data->time_to_eat = ft_atoi(argv[2]);
	data->time_to_sleep = ft_atoi(argv[3]);
	data->nb_of_meals = ft_atoi(argv[4]);
	if (is_even(data->total_philo_nb))
		data->time_to_think = data->time_to_eat - data->time_to_sleep;
	else
		data->time_to_think = data->time_to_eat + data->time_to_sleep;
	printf("time to think = %d\n", data->time_to_think);
	return (0);
}

int	main(int argc, char **argv)
{
	pthread_t	*philos;
	t_mutex		*forks;
	t_philo		data;
	int			philos_nb;

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
