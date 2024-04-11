/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 12:21:14 by kipouliq          #+#    #+#             */
/*   Updated: 2024/04/11 18:13:22 by kipouliq         ###   ########.fr       */
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

int	check_end_exec(t_philo *data)
{
	pthread_mutex_lock(data->end_exec_lock);
	if (data->end_exec)
    {
	    pthread_mutex_unlock(data->end_exec_lock);
		return (1);
    }
    pthread_mutex_unlock(data->end_exec_lock);	
	return (0);
}

int	check_time_to_die(long int *last_meal, t_timeval starting_time, int ttd)
{
	long int	current_time;

	current_time = get_time_elapsed(&starting_time);
    // printf("check death = %ld\n", current_time - *last_meal);
	if (current_time - *last_meal > ttd)
		return (0);
	return (1);
}

int	usleep_and_check(t_philo *data, long int *last_meal, int time)
{
	int	i;
	int	limit;

	i = 0;
	limit = time / 10;
	while (i < limit)
	{
		usleep(10000);
		if (!check_time_to_die(last_meal, data->starting_time,
				data->time_to_die))
			return (-1); // a changer
		if (check_end_exec(data))
			return (-1);
		i++;
	}
	return (0);
}

int	eat_and_check(t_philo *data)
{
	int	i;
	int	limit;

	i = 0;
	limit = data->time_to_eat / 10;
	while (i < limit)
	{
		usleep(10000);
		if (check_end_exec(data))
			return (1);
		i++;
	}
	return (0);
}

int	unlock_forks(t_mutex *fork_1, t_mutex *fork_2)
{
	if (fork_1)
		pthread_mutex_unlock(fork_1);
	if (fork_2)
		pthread_mutex_unlock(fork_2);
	return (-1);
}

int	death_routine(t_philo *data, t_mutex *fork_1, t_mutex *fork_2,
		int philo_index)
{
	if (data->total_philo_nb == 1)
	{
		pthread_mutex_lock(data->deaths_lock);
		data->philo_deaths[0] = 1;
		pthread_mutex_unlock(data->deaths_lock);
		return (0);
	}
	if (fork_1)
		pthread_mutex_unlock(fork_1);
	if (fork_2)
		pthread_mutex_unlock(fork_2);
	pthread_mutex_lock(data->deaths_lock);
	data->philo_deaths[philo_index - 1] = 1;
	pthread_mutex_unlock(data->deaths_lock);
	return (-1);
}

int	print_fork(t_philo *data, int philo_index)
{
	pthread_mutex_lock(data->write_lock);
	if (check_end_exec(data))
	{
		pthread_mutex_unlock(data->write_lock);
		return (0);
	}
	printf("%ld %d has taken a fork\n", get_time_elapsed(&data->starting_time),
		philo_index);
	pthread_mutex_unlock(data->write_lock);
	return (0);
}

int	sleep_routine(t_philo *data, long int *last_meal, int philo_index)
{
	pthread_mutex_lock(data->write_lock);
	if (check_end_exec(data))
    {
        pthread_mutex_unlock(data->write_lock);
		return (0);
    }
	printf("%ld %d is sleeping\n", get_time_elapsed(&data->starting_time),
		philo_index);
	pthread_mutex_unlock(data->write_lock);
	usleep_and_check(data, last_meal, data->time_to_sleep);
	return (0);
}

int	think_routine(t_philo *data, long int *last_meal, int philo_index)
{
	pthread_mutex_lock(data->write_lock);
	if (check_end_exec(data))
	{
		pthread_mutex_unlock(data->write_lock);
		return (-1);
	}
	printf("%ld %d is thinking\n", get_time_elapsed(&data->starting_time),
		philo_index);
	pthread_mutex_unlock(data->write_lock);
    if (data->time_to_think == 0 || data->time_to_think < 0)
    {
        printf("coucou\n");
        usleep(1000);
    }
    // {
    //     if (usleep_and_check(data, last_meal, 10) == -1)
    //         return (death_routine(data, NULL, NULL, philo_index));
    // }
    else
    {
	    if (usleep_and_check(data, last_meal, data->time_to_think) == -1)
		    return (death_routine(data, NULL, NULL, philo_index));
    }
	return (0);
}

int	lock_forks(t_mutex *fork_1, t_mutex *fork_2, t_philo *data, int philo_index)
{
	pthread_mutex_lock(fork_1);
	if (check_end_exec(data))
	{
		pthread_mutex_unlock(fork_1);
		return (-1);
	}
	print_fork(data, philo_index);
	pthread_mutex_lock(fork_2);
	if (check_end_exec(data))
	{
        pthread_mutex_unlock(fork_1);
		pthread_mutex_unlock(fork_2);
		return (-1);
	}
	print_fork(data, philo_index);
	return (0);
}

int	eat_routine(t_philo *data, long int *last_meal, int philo_index)
{
	t_mutex	*fork_1;
	t_mutex	*fork_2;

    // printf("philo %d last meal = %ld\n", philo_index, *last_meal);
	fork_1 = &data->forks[philo_index - 1];
	if (philo_index == data->total_philo_nb)
		fork_2 = &data->forks[0];
	else
		fork_2 = &data->forks[philo_index];
	if (is_even(philo_index))
	{
		if (lock_forks(fork_1, fork_2, data, philo_index) == -1)
			return (-1);
	}
	else
	{
		if (lock_forks(fork_2, fork_1, data, philo_index) == -1)
			return (-1);
	}
	if (!check_time_to_die(last_meal, data->starting_time, data->time_to_die))
		return (death_routine(data, fork_1, fork_2, philo_index));
	if (check_end_exec(data))
	{
		pthread_mutex_unlock(fork_1);
		pthread_mutex_unlock(fork_2);
		return (-1);
	}
	pthread_mutex_lock(data->write_lock);
	printf("%ld %d is eating\n", get_time_elapsed(&data->starting_time),
		philo_index);
	pthread_mutex_unlock(data->write_lock);
	*last_meal = get_time_elapsed(&data->starting_time);
	eat_and_check(data);
	pthread_mutex_unlock(fork_1);
	pthread_mutex_unlock(fork_2);
	return (0);
}

int	only_one_philo_routine(t_philo *data)
{
	t_mutex	*fork;

	fork = &data->forks[0];
	pthread_mutex_lock(fork);
	print_fork(data, 1);
	usleep(data->time_to_die * 1000);
	death_routine(data, NULL, NULL, 0);
	pthread_mutex_unlock(fork);
	return (0);
}

int	philo_routine(t_philo *data)
{
	int			philo_index;
	int			meals_finished;
	int			max_meals;
	long int	last_meal;

	meals_finished = 0;
	last_meal = 0;
	pthread_mutex_lock(data->nb_of_meals_lock);
	max_meals = data->nb_of_meals;
	pthread_mutex_unlock(data->nb_of_meals_lock);
	pthread_mutex_lock(data->philo_count_lock);
	data->philo_nb += 1;
	philo_index = data->philo_nb;
	pthread_mutex_unlock(data->philo_count_lock);
	if (is_even(philo_index))
		usleep((data->time_to_eat * 1000) - 100);
	while (1)
	{
		if (check_end_exec(data))
			return (0);
		if (data->total_philo_nb == 1)
			return (only_one_philo_routine(data));
		if (meals_finished == max_meals)
		{
			pthread_mutex_lock(data->f_meals_lock);
			data->philo_f_meals[philo_index - 1] = 1;
			pthread_mutex_unlock(data->f_meals_lock);
		}
		if (eat_routine(data, &last_meal, philo_index) == -1)
			return (-1);
		if (sleep_routine(data, &last_meal, philo_index) == -1)
			return (-1);
		if (think_routine(data, &last_meal, philo_index) == -1)
			return (-1);
		meals_finished += 1;
	}
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

int	init_mutexes(t_philo *data)
{
	t_mutex	*philo_nb;
	t_mutex	*meals_nb;
	t_mutex	*f_eating;
	t_mutex	*deaths;
	t_mutex	*write;
	t_mutex	*end_exec;

	philo_nb = malloc(sizeof(t_mutex));
	meals_nb = malloc(sizeof(t_mutex));
	f_eating = malloc(sizeof(t_mutex));
	deaths = malloc(sizeof(t_mutex));
	write = malloc(sizeof(t_mutex));
	end_exec = malloc(sizeof(t_mutex));
	if (!philo_nb || !meals_nb || !f_eating || !write || !deaths || !end_exec)
		return (-1);
	if (pthread_mutex_init(philo_nb, NULL) == -1)
		return (-1);
	if (pthread_mutex_init(meals_nb, NULL) == -1) // if nb of meals
		return (-1);
	if (pthread_mutex_init(f_eating, NULL) == -1)
		return (-1);
	if (pthread_mutex_init(write, NULL) == -1)
		return (-1);
	if (pthread_mutex_init(end_exec, NULL) == -1)
		return (-1);
	if (pthread_mutex_init(deaths, NULL) == -1)
		return (-1);
	data->philo_count_lock = philo_nb;
	data->nb_of_meals_lock = meals_nb;
	data->f_meals_lock = f_eating;
	data->deaths_lock = deaths;
	data->write_lock = write;
	data->end_exec_lock = end_exec;
	return (0);
}

int	init_philo_data(int argc, char **argv, t_mutex *forks_tab, t_philo *data)
{
	data->philo_nb = 0;
	data->end_exec = 0;
	data->forks = forks_tab;
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
	if (init_mutexes(data) == -1)
		return (-1);
    printf("time to think = %d\n", data->time_to_think);
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
		if (!data->philo_deaths)
			return (-1);
		i = -1;
		while (++i < data->total_philo_nb)
			data->philo_f_meals[i] = 0;
	}
	else
		data->philo_f_meals = NULL;
	return (0);
}

int	are_all_philos_fed(t_philo *data)
{
	int	i;

	i = -1;
	while (++i < data->total_philo_nb)
	{
		pthread_mutex_lock(data->f_meals_lock);
		if (!data->philo_f_meals[i])
		{
			pthread_mutex_unlock(data->f_meals_lock);
			return (0);
		}
		pthread_mutex_unlock(data->f_meals_lock);
	}
	return (1);
}

int	monitor_philos(t_philo *data)
{
	int	i;

	while (1)
	{
		i = -1;
		while (++i < data->total_philo_nb)
		{
			pthread_mutex_lock(data->deaths_lock);
			if (data->philo_deaths[i])
			{
				pthread_mutex_lock(data->end_exec_lock);
				data->end_exec = 1;
				pthread_mutex_unlock(data->end_exec_lock);
				pthread_mutex_unlock(data->deaths_lock);
				pthread_mutex_lock(data->write_lock);
				printf("%ld %d died\n", get_time_elapsed(&data->starting_time),
					i + 1);
				pthread_mutex_unlock(data->write_lock);
				return (0);
			}
			pthread_mutex_unlock(data->deaths_lock);
		}
		if (data->nb_of_meals != -1 && are_all_philos_fed(data))
		{
			pthread_mutex_lock(data->end_exec_lock);
			data->end_exec = 1;
			pthread_mutex_unlock(data->end_exec_lock);
			return (0);
		}
		usleep(100);
	}
}

int	ft_destroy_mutexes(t_philo *data)
{
	int	i;

	i = -1;
	while (++i < data->total_philo_nb)
		pthread_mutex_destroy(&data->forks[i]);
	pthread_mutex_destroy(data->philo_count_lock);
	free(data->philo_count_lock);
	pthread_mutex_destroy(data->write_lock);
	free(data->write_lock);
	pthread_mutex_destroy(data->deaths_lock);
	free(data->deaths_lock);
	pthread_mutex_destroy(data->f_meals_lock);
	free(data->f_meals_lock);
	pthread_mutex_destroy(data->nb_of_meals_lock);
	free(data->nb_of_meals_lock);
	pthread_mutex_destroy(data->end_exec_lock);
	free(data->end_exec_lock);
	return (0);
}

int	free_everything(t_philo *data)
{
	ft_destroy_mutexes(data);
	free(data->forks);
	free(data->philo_deaths);
	if (data->philo_f_meals)
		free(data->philo_f_meals);
	return (0);
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
	philos_nb = ft_atoi(argv[0]);
	forks = create_forks(philos_nb);
	if (!forks)
		return (-1);
	init_philo_data(argc, argv, forks, &data);
	init_tabs(&data);
	gettimeofday(&data.starting_time, NULL);
	philos = create_philos(philos_nb, &data);
	if (!philos)
		return (-1);
	monitor_philos(&data);
	wait_philos(philos, data.total_philo_nb);
	free_everything(&data);
	free(philos);
	return (0);
}
