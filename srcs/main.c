/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 12:21:14 by kipouliq          #+#    #+#             */
/*   Updated: 2024/04/17 14:31:45 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

pthread_t	*create_philos(t_philo *data)
{
	pthread_t	*philos_tab;
	int			i;
	int			error;

	error = 0;
	philos_tab = malloc(sizeof(pthread_t) * (data->total_philo_nb));
	if (!philos_tab)
		return (NULL);
	i = -1;
	while (++i < data->total_philo_nb)
	{
		if (data->total_philo_nb == 1)
			error = pthread_create(&philos_tab[i], NULL,
					(void *)only_one_philo_routine, data);
		else
			error = pthread_create(&philos_tab[i], NULL, (void *)philo_routine,
					data);
		if (error)
			return (NULL);
	}
	return (philos_tab);
}

t_mutex	*create_mutex_tab(int mtx_nb)
{
	t_mutex	*mtx_tab;
	int		i;
	int		error;

	error = 0;
	mtx_tab = malloc(sizeof(t_mutex) * (mtx_nb));
	if (!mtx_tab)
		return (NULL);
	i = -1;
	while (++i < mtx_nb)
	{
		error = pthread_mutex_init(&mtx_tab[i], NULL);
		if (error)
		{
			free(mtx_tab);
			return (NULL);
		}
	}
	return (mtx_tab);
}

int	main(int argc, char **argv)
{
	pthread_t	*philos;
	t_philo		data;

	if (argc != 5 && argc != 6)
		return (-1);
	argv += 1;
	if (init_philo_data(argc, argv, &data) == -1)
		return (-1);
	if (init_tabs(&data) == -1)
		return (-1);
	philos = create_philos(&data);
	if (!philos)
	{
		fill_death_tab(&data);
		free_everything(&data);
		free(philos);
		return (-1);
	}
	monitor_philos(&data);
	wait_philos(philos, data.total_philo_nb);
	free_everything(&data);
	free(philos);
	return (0);
}
