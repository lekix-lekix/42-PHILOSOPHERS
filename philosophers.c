/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 12:21:14 by kipouliq          #+#    #+#             */
/*   Updated: 2024/03/22 17:07:15 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <stdio.h>
#include <pthread.h>
#include "./philo.h"

// ./philo 4 100 100 100 5

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

int create_philos(pthread_t *philos)
{
    
}

int main(int argc, char **argv)
{
    pthread_t   *philos_ids;
    int         philos_nb;
    
    if (argc != 5)
        return ;
    philos_nb = ft_atoi(argv[1]);
    philos_ids = malloc(sizeof(pthread_t) * (philos_nb + 1));
    if (!philos_ids)
        return (-1);
    philos_ids[philos_nb] = NULL;
    create_philos(philos_ids);
}
