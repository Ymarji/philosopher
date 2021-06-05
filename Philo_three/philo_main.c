/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarji <ymarji@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 13:27:09 by ymarji            #+#    #+#             */
/*   Updated: 2021/06/05 12:06:06 by ymarji           ###   ########.fr       */
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
	var->totalmeal++;
	usleep(var->arg.time_to_eat * 1000);
	phil->nmbrofmeal--;
	if (phil->nmbrofmeal == 0)
	{
		sem_post(var->meal);
	}
}

void	p_msg(t_var *var, char *msg, long arg, int nbr)
{
	sem_wait(var->print_lock);
	printf(msg, arg, nbr);
	sem_post(var->print_lock);
}

int	init(t_var *var)
{
	sem_unlink("/g_lock");
	sem_unlink("/p_lock");
	sem_unlink("/d_lock");
	var->mt = sem_open("/g_lock", O_CREAT, 0664, 1);
	var->print_lock = sem_open("/p_lock", O_CREAT, 0664, 1);
	var->death_lock = sem_open("/d_lock", O_CREAT, 0664, 1);
	if (var->mt == SEM_FAILED || var->print_lock == SEM_FAILED
		|| var->death_lock == SEM_FAILED)
	{
		ft_putendl_fd("Error: init lock !!\n", 2);
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
		sem_wait(var->mt);
		creat_pross(var);
		sem_wait(var->mt);
		ft_free(var, END);
	}
	return (0);
}
