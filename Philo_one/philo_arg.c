/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_arg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarji <ymarji@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/04 11:45:04 by ymarji            #+#    #+#             */
/*   Updated: 2021/06/04 18:04:23 by ymarji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philosophers.h"

void init_philo(t_var *var)
{
	int i;

	i = 0;
	while (i < var->n_ph)
	{
		var->phil[i].index = i;
		var->phil[i].nbr = i + 1;
		var->phil[i].right = i;
		var->phil[i].left = (i + 1) % var->n_ph;
		var->phil[i].var = var;
		var->phil[i].nmbrofmeal = var->arg.num_eat;
		i++;
	}
}

int cheack_args(t_var *var, char *av)
{
	if (var->n_ph < 0 || var->arg.time_to_die < 0 || var->arg.time_to_eat < 0 || var->arg.time_to_sleep < 0)
	{
		ft_putendl_fd("Error", 2);
		return (1);
	}
	if (av)
	{
		if (var->arg.num_eat < 0)
		{
			ft_putendl_fd("Error", 2);
			return (1);
		}
	}
	return (0);
}

int get_args(t_var *var, char **av)
{
	int i;
	t_philo *new;

	i = 1;
	while (av[i])
	{
		if (!ft_isdigit_str(av[i]))
		{
			ft_putendl_fd("Error", 2);
			return (1);
		}
		i++;
	}
	var->n_ph = ft_atoi(av[1]);
	var->arg.time_to_die = ft_atoi(av[2]);
	var->arg.time_to_eat = ft_atoi(av[3]);
	var->arg.time_to_sleep = ft_atoi(av[4]);
	if (av[5])
		var->arg.num_eat = ft_atoi(av[5]);
	else
		var->arg.num_eat = -1;
	return (cheack_args(var, av[5]));
}

long get_time(int ind, long start)
{
	struct timeval time;
	long count;

	gettimeofday(&time, NULL);
	count = time.tv_sec * 1000 + time.tv_usec / 1000;
	if (ind == 0)
		return (count);
	else
		return (count - start);
}

void ft_free(t_var *var, int step)
{
	int i;

	i = -1;
	if (var)
	{
		if (step == END)
		{
			while (++i < var->n_ph)
				pthread_mutex_destroy(&var->fork[i]);
			if (var->phil)
				free(var->phil);
		}
		free(var);
	}
}
