#include "Philosophers.h"

pthread_mutex_t mt, mt1;

void	init_philo(t_var *var)
{
	int i;

	i = 0;
	while (i < var->num_of_philo)
	{
		var->phil[i].philo_nbr = i;
		var->phil[i].right = i;
		var->phil[i].left = (i + 1) % var->num_of_philo;
		var->phil[i].var = var;
		// if (i == (int)(var->num_of_philo / 2))
		// {
		// 	var->phil[i].right = var->phil[i].left;
		// 	var->phil[i].left = i;
		// }
		printf("|%d| |%d| \n",var->phil[i].right ,var->phil[i].left );
		i++;
	}
}

int	get_args(t_var *var,char **av)
{
	int i;
	t_philo *new;

	i = 1;
	while (av[i])
	{
		if (!ft_isdigit_str(av[i]))
		{
			ft_putendl_fd("Error", 2);
			return (0);
		}
		i++;
	}
	var->num_of_philo = ft_atoi(av[1]);
	var->arg.time_to_die  = ft_atoi(av[2]);
	var->arg.time_to_eat  = ft_atoi(av[3]);
	var->arg.time_to_sleep  = ft_atoi(av[4]);
	return (1);
}

void	*philosopher_life(void *arg)
{
	t_var *var;
	t_philo *t;
	int		i = 0;

	t = (t_philo *)arg;
	var = t->var;
	while (1)
	{
		pthread_mutex_lock(&var->fork[t->right]);
				printf("------->> {%d} philo pick {%d}\n", t->philo_nbr, t->right);
		pthread_mutex_lock(&var->fork[t->left]);
				printf("------->> {%d} philo pick {%d}\n", t->philo_nbr, t->left);
		printf("%d philo start with %d %d\n",t->philo_nbr, t->right, t->left);
		printf("%d philo eating\n",t->philo_nbr);
		usleep(2 * 1000000);
		printf("%d philo stop with %d %d\n",t->philo_nbr, t->right, t->left);
		pthread_mutex_unlock(&var->fork[t->right]);
		pthread_mutex_unlock(&var->fork[t->left]);
		printf("%d philo sleep\n",t->philo_nbr);
		usleep(60 * 1000);
	}
	return (NULL);
}

void	creat_thread(t_var *var)
{
	int i;

	var->fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t ) * (var->num_of_philo)); 
	var->phil = (t_philo *)malloc(sizeof(t_philo) * (var->num_of_philo));
	init_philo(var);
	i = -1;
	while (++i < var->num_of_philo)
		pthread_mutex_init(&(var->fork[i]), NULL);

	i = 0;
	while (i < var->num_of_philo)
	{
		pthread_create(&var->tid, NULL, &philosopher_life, &var->phil[i]);
		pthread_detach(var->tid);
		usleep(100);
		i++;
	}
}

int main(int ac, char **av)
{
	t_var *var;

	var = (t_var *)malloc(sizeof(t_var));
	pthread_mutex_init(&mt, NULL);
	pthread_mutex_init(&mt1, NULL);
	var->phil = NULL;
	if (ac <= 4 || ac > 6)
		return (1);
	get_args(var, av);
	pthread_mutex_lock(&mt);
	creat_thread(var);
	pthread_mutex_lock(&mt);
	return (0);
}
