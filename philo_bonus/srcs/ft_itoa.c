/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 13:58:28 by kipouliq          #+#    #+#             */
/*   Updated: 2024/04/17 15:53:45 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t	i;

	i = -1;
	while (++i < n)
		*((char *)s + i) = c;
	return (s);
}

static int	ft_find_nb_len(int nb)
{
	int	digits;

	if (nb == 0)
		return (1);
	digits = 0;
	if (nb < 0)
	{
		digits++;
		nb = -nb;
	}
	while (nb != 0)
	{
		nb /= 10;
		digits++;
	}
	return (digits);
}

static void	ft_convert(char *str, long int nb, int *i)
{
	if (nb < 0)
	{
		str[*i] = '-';
		nb = -nb;
		*i += 1;
	}
	if (nb >= 0 && nb <= 9)
	{
		str[*i] = nb + '0';
		*i += 1;
	}
	if (nb > 9)
	{
		ft_convert(str, nb / 10, i);
		str[*i] = nb % 10 + '0';
		*i += 1;
	}
}

char	*ft_itoa(int n)
{
	long int	nb;
	int			n_len;
	int			i;
	char		*str;

	nb = n;
	n_len = ft_find_nb_len(nb);
	str = malloc(sizeof(char) * (n_len + 1));
	if (!str)
		return (NULL);
	str = ft_memset(str, '\0', n_len + 1);
	if (!str)
		return (NULL);
	i = 0;
	ft_convert(str, nb, &i);
	return (str);
}
