#include "Philosophers.h"


// int		ft_lstsize_m(t_philo *lst)
// {
// 	int cp;

// 	cp = 0;
// 	while (lst != NULL)
// 	{
// 		cp++;
// 		lst = lst->next;
// 	}
// 	return (cp);
// }

// t_philo	*ft_lstnew_m(int content)
// {
// 	t_philo	*new;

// 	new = (t_philo *)malloc(sizeof(t_philo));
// 	if (new == NULL)
// 		return (0);
// 	new->philo_nbr = content;
// 	new->next = NULL;
// 	return (new);
// }

// t_philo	*ft_lstlast_m(t_philo *lst)
// {
// 	t_philo *tmp;

// 	tmp = lst;
// 	if (tmp == NULL)
// 		return (NULL);
// 	while (tmp->next != NULL)
// 		tmp = tmp->next;
// 	return (tmp);
// }

// void	ft_lstadd_back_m(t_philo **alst, t_philo *new)
// {
// 	t_philo	*p;

// 	if (*alst == NULL)
// 	{
// 		*alst = new;
// 		return ;
// 	}
// 	p = ft_lstlast_m(*alst);
// 	p->next = new;
// 	// new->prev = p;
// }

// void	ft_deletenode(t_philo **head_ref, t_philo *del)
// {
// 	if (*head_ref == NULL || del == NULL)
// 		return ;
// 	if (*head_ref == del)
// 		*head_ref = del->next;
// 	if (del->next != NULL)
// 		del->next->prev = del->prev;
// 	if (del->prev != NULL)
// 		del->prev->next = del->next;
// 	// free(del->ident);
// 	// free(del->value);
// 	free(del);
// }
