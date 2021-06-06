/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarji <ymarji@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 13:27:09 by ymarji            #+#    #+#             */
/*   Updated: 2021/06/06 10:43:40 by ymarji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philosophers.h"

void	sleeping(t_var *var, t_philo *phil)
{
	p_msg(var, "%lu %d is sleeping\n", get_time(1, var->start), phil->nbr);
	usleep(var->arg.time_to_sleep * 1000);
}

void	eat(t_var *var, t_philo *phil)
{
	p_msg(var, "%lu %d is eating\n", get_time(1, var->start), phil->nbr);
	phil->timofdeath = get_time(0, 0) + var->arg.time_to_die;
	usleep(var->arg.time_to_eat * 1000);
	var->totalmeal++;
}

void	p_msg(t_var *var, char *msg, long arg, int nbr)
{
	pthread_mutex_lock(&var->print_lock);
	printf(msg, arg, nbr);
	pthread_mutex_unlock(&var->print_lock);
}

int	init(t_var *var)
{
	if (pthread_mutex_init(&var->mt, NULL)
		|| pthread_mutex_init(&var->print_lock, NULL)
		|| pthread_mutex_init(&var->death_lock, NULL))
	{
		ft_putendl_fd("Error: init mutex !!\n", 2);
		return (0);
	}
	var->phil = NULL;
	return (1);
}

int	main(int ac, char **av)
{
	t_var	*var;
	int		i;

	i = -1;
	var = (t_var *)malloc(sizeof(t_var));
	if (ac <= 4 || ac > 6 || !init(var))
	{
		ft_free(var, START);
		return (1);
	}
	if (!get_args(var, av))
	{
		pthread_mutex_lock(&var->mt);
		creat_thread(var);
		pthread_mutex_lock(&var->mt);
		pthread_mutex_destroy(&var->mt);
		pthread_mutex_destroy(&var->death_lock);
		pthread_mutex_destroy(&var->print_lock);
		ft_free(var, END);
	}
	return (0);
}
