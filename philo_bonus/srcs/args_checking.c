/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_checking.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 14:20:10 by kipouliq          #+#    #+#             */
/*   Updated: 2024/04/17 15:34:37 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return (0);
		i++;
	}
	return (1);
}

int	ft_check_overflow(char *str)
{
	int		nb;
	char	*test_str;

	nb = ft_atoi(str);
	test_str = ft_itoa(nb);
	if (!test_str)
		return (0);
	if (!ft_strcmp(str, test_str))
	{
		free(test_str);
		return (0);
	}
	free(test_str);
	return (1);
}

int	str_digits_check(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (!is_digit(str[i]))
			return (0);
	}
	return (1);
}

int	arg_check(char *str)
{
	int	arg_len;

	arg_len = ft_strlen(str);
	if (arg_len > 11)
		return (0);
	if (!str_digits_check(str))
		return (0);
	if ((arg_len == 10 || arg_len == 11) && !ft_check_overflow(str))
		return (0);
	return (1);
}

int	check_args(char **argv)
{
	int	i;

	i = -1;
	while (argv[++i])
	{
		if (!arg_check(argv[i]))
			return (0);
	}
	return (1);
}
