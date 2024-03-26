/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 12:21:14 by kipouliq          #+#    #+#             */
/*   Updated: 2024/03/26 18:21:04 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

// ./philo 4 100 100 100 5

t_time	get_time_elapsed(t_timeval *starting_time)
{
	t_timeval	current_time;
	t_time		time_elapsed;

	gettimeofday(&current_time, NULL);
	time_elapsed.time_sec = current_time.tv_sec - starting_time->tv_sec;
	time_elapsed.time_ms = ((float)current_time.tv_usec
			- (float)starting_time->tv_usec) / 1000;
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

int	check_forks(t_philo *data)
{
}

void	philo_routine(t_philo *data)
{
	int	meals_finished;

	meals_finished = 0;
	while (meals_finished < data->nb_of_meals)
	{
	}
}

pthread_t	*create_philos(int philos_nb, t_philo **all_data)
{
	pthread_t	*philos_tab;
	t_philo		*data;
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

t_philo 	**create_philo_data_structs(int philos_nb, char **argv,
		pthread_mutex_t *forks_tab)
{
	t_philo **data_structs;
    t_philo *philo_data;
    int	i;

    data_structs = malloc(sizeof(t_philo *) * (ft_atoi(argv[0])));
    if (!data_structs)
        return (NULL);
	data->forks = forks_tab;
	data->time_to_die = ft_atoi(argv[1]);
	data->time_to_eat = ft_atoi(argv[2]);
	data->time_to_sleep = ft_atoi(argv[3]);
	data->nb_of_meals = ft_atoi(argv[4]);
	return (0);
}

int	main(int argc, char **argv)
{
	t_philo			**philos_data;
	pthread_t		*philos;
	pthread_mutex_t	*forks;
	int				philos_nb;

	if (argc != 6)
		return (-1);
	argv += 1;
	philos_nb = ft_atoi(argv[0]);
	forks = create_forks(philos_nb);
	if (!forks)
		return (-1);
	philos_data = create_philo_data_structs(argv, forks);
	philos = create_philos(philos_nb, philos_data);
	if (!philos)
	{
		return (-1);
	}
	printf("pthread_join = %d\n", wait_philos(philos, philos_nb));
	return (0);
}
