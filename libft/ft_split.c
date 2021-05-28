/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarji <ymarji@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/23 10:52:55 by ymarji            #+#    #+#             */
/*   Updated: 2019/11/07 09:56:16 by ymarji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	bad_spot(char **tab, int i)
{
	while (i)
	{
		free(tab[i]);
		i--;
	}
	free(tab);
}

int		c_split(char const *str, char c)
{
	int	i;
	int cp_word;
	int is_num;

	is_num = 0;
	cp_word = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			is_num = 0;
		else
		{
			if (is_num == 0)
			{
				cp_word++;
				is_num = 1;
			}
		}
		i++;
	}
	return (cp_word);
}

char	*p_split(char const *str, int *n, char c)
{
	int		i;
	int		start;
	int		end;
	char	*split;

	i = *n;
	while (str[i] == c)
		i++;
	start = i;
	while (str[i] != c && str[i])
		i++;
	end = i;
	*n = end;
	if (!(split = (char *)malloc(end - start + 1)))
		return (NULL);
	i = 0;
	while (start < end)
	{
		split[i] = str[start];
		i++;
		start++;
	}
	split[i] = '\0';
	return (split);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	int		pos;
	char	**split_t;

	if (s == NULL)
		return (NULL);
	split_t = (char **)malloc((1 + c_split(s, c)) * sizeof(char *));
	if (split_t == NULL)
		return (NULL);
	i = 0;
	pos = 0;
	while (i < c_split(s, c))
	{
		if (!(split_t[i] = p_split(s, &pos, c)))
		{
			bad_spot(split_t, i);
			return (NULL);
		}
		i++;
	}
	split_t[i] = NULL;
	return (split_t);
}
