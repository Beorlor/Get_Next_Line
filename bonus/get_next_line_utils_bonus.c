/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <jedurand@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 07:57:36 by nikito            #+#    #+#             */
/*   Updated: 2023/10/11 00:52:27 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/get_next_line_bonus.h"

/* Looks for a newline character in the given linked list. */

int	found_newline(int fd, t_list *stash)
{
	int		i;

	if (stash == NULL)
		return (0);
	while (stash && stash->fd != fd)
		stash = stash->next;
	i = 0;
	while (stash && stash->content[i])
	{
		if (stash->content[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

/* Returns a pointer to the last node in our stash */

t_list	*ft_lst_get_last(t_list *stash)
{
	t_list	*current;

	current = stash;
	if (current == NULL)
		return (current);
	while (current->next)
		current = current->next;
	return (current);
}

/* Calculates the number of chars in the current line, including the trailing
 * \n if there is one, and allocates memory accordingly. */

void	generate_line(int fd, char **line, t_list *stash)
{
	int	i;
	int	len;

	len = 0;
	while (stash)
	{
		i = 0;
		if (stash->fd == fd)
		{
			while (stash->content[i] && stash->content[i] != '\n')
			{
				len++;
				i++;
			}
			if (stash->content[i] == '\n')
				len++;
		}
		stash = stash->next;
	}
	*line = malloc(sizeof(char) * (len + 1));
}

/* Frees the entire stash. */

void	free_stash(int fd, t_list *stash)
{
	t_list	*current;
	t_list	*next;

	current = stash;
	while (current)
	{
		if (current->fd == fd)
		{
			free(current->content);
			free(current->fd);
			next = current->next;
			free(current);
			current = next;
		}
		else
			current = current->next;
	}
}

int	ft_strlen(const char *str)
{
	int	len;

	len = 0;
	while (*(str++))
		len++;
	return (len);
}
