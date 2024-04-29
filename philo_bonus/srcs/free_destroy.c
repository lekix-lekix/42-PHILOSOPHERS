/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_destroy.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 16:27:13 by kipouliq          #+#    #+#             */
/*   Updated: 2024/04/29 14:54:52 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

int	unlink_semaphores(void)
{
	sem_unlink("philo_index");
	sem_unlink("forks");
	sem_unlink("finished_meals");
	sem_unlink("deaths");
	sem_unlink("write");
	sem_unlink("killing");
	return (0);
}

int	close_semaphores(t_sem *locks)
{
	sem_close(locks->forks);
	sem_close(locks->deaths);
	sem_close(locks->finished_meals);
	sem_close(locks->write);
	sem_close(locks->philo_index);
	sem_close(locks->killing);
	return (0);
}

int	free_stuff(t_philo *data)
{
	close_semaphores(data->locks);
	unlink_semaphores();
	free(data->philos_pids);
	free(data->locks);
	return (0);
}
