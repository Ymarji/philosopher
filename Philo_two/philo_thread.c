/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_thread.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarji <ymarji@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/04 18:11:49 by ymarji            #+#    #+#             */
/*   Updated: 2021/06/06 10:41:12 by ymarji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philosophers.h"

void	*philosopher_life(void *arg)
{
	t_var	*var;
	t_philo	*ph;
	int		i;

	i = 0;
	ph = (t_philo *)arg;
	var = ph->var;
	ph->timofdeath = get_time(0, 0) + var->arg.time_to_die;
	life_end(var, ph);
	while (1)
	{
		sem_wait(var->fork);
		p_msg(var, "%lu %d has taken a fork\n",
			get_time(1, var->start), ph->nbr);
		sem_wait(var->fork);
		p_msg(var, "%lu %d has taken a fork\n",
			get_time(1, var->start), ph->nbr);
		eat(var, ph);
		sem_post(var->fork);
		sem_post(var->fork);
		sleeping(var, ph);
		p_msg(var, "%lu %d is thinking\n", get_time(1, var->start), ph->nbr);
	}
	return (NULL);
}

int	check_meal(t_var *var, int meal, int totalmeal)
{
	if (var->arg.num_eat != -1 && meal >= totalmeal)
	{
		sem_wait(var->print_lock);
		printf("SIMULATION DONE\n");
		sem_post(var->mt);
		return (1);
	}
	return (0);
}

void	*to_die(void *arg)
{
	t_philo	*ph;
	t_var	*var;

	ph = (t_philo *)arg;
	var = ph->var;
	while (1)
	{
		sem_wait(var->death_lock);
		if (get_time(0, 0) > ph->timofdeath)
		{
			sem_wait(var->print_lock);
			printf("%lu %d died\n", get_time(1, var->start), ph->nbr);
			sem_post(var->mt);
			break ;
		}
		if (check_meal(var, var->totalmeal, var->arg.num_eat * var->n_ph) == 1)
			break ;
		sem_post(var->death_lock);
		usleep(500);
	}
	return (NULL);
}

void	creat_thread(t_var *var)
{
	int	i;

	sem_unlink("/fork");
	var->fork = sem_open("/fork", O_CREAT, 777, var->n_ph);
	var->phil = (t_philo *)malloc(sizeof(t_philo) * (var->n_ph));
	init_philo(var);
	i = 0;
	var->start = get_time(0, 0);
	while (i < var->n_ph)
	{
		pthread_create(&var->tid, NULL, &philosopher_life, &var->phil[i]);
		pthread_detach(var->tid);
		usleep(100);
		i++;
	}
}

void	life_end(t_var *var, t_philo *phil)
{
	pthread_create(&var->tid, NULL, &to_die, phil);
	pthread_detach(var->tid);
}
