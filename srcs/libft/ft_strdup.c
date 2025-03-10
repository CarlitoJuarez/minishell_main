/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhocsak <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 14:35:00 by bhocsak           #+#    #+#             */
/*   Updated: 2024/04/09 16:48:20 by bhocsak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	size_t	i;
	size_t	j;
	char	*ptr;

	i = 0;
	j = 0;
	while (s[j] != '\0')
		j++;
	ptr = ft_calloc(sizeof(char), (j + 1));
	if (!ptr)
		return (perror("malloc failed!"), NULL);
	ptr[j] = 0;
	while (i < j)
	{
		ptr[i] = s[i];
		i++;
	}
	return (ptr);
}
