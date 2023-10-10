/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <jedurand@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 07:57:36 by nikito            #+#    #+#             */
/*   Updated: 2023/10/11 00:52:12 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/get_next_line_bonus.h"

char	*get_next_line(int fd)
{
	static t_list	*stash = NULL;
	char			*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, &line, 0) < 0)
		return (NULL);
	line = NULL;
	// 1. read from fd and add to linked list
	read_and_stash(fd, &stash);
	if (stash == NULL)
		return (NULL);
	// 2. extract from stash to line
	extract_line(fd, stash, &line);
	// 3. clean up stash
	clean_stash(fd, &stash);
	if (line[0] == '\0')
	{
		free_stash(fd, stash);
		stash = NULL;
		free(line);
		return (NULL);
	}
	return (line);
}

/* Adds the content of our buffer to the end of our stash */

static void	add_to_stash(t_list **stash, char *buff, int readed, int fd)
{
	int		i;
	t_list	*last;
	t_list	*new_node;

	new_node = malloc(sizeof(t_list));
	if (new_node == NULL)
		return ;
	new_node->next = NULL;
	new_node->fd = malloc(sizeof(int));
	if (new_node->fd == NULL)
		return ;
	new_node->fd = fd; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	new_node->content = malloc(sizeof(char) * (readed + 1));
	if (new_node->content == NULL)
		return ;
	i = 0;
	while (buff[i] && i < readed)
	{
		new_node->content[i] = buff[i];
		i++;
	}
	new_node->content[i] = '\0';
	if (*stash == NULL)
	{
		*stash = new_node;
		return ;
	}
	last = ft_lst_get_last(*stash);
	last->next = new_node;
}

/* Uses read() to add characters to the stash */

void	read_and_stash(int fd, t_list **stash)
{
	char	*buff;
	int		readed;

	readed = 1;
	while (!found_newline(fd, *stash) && readed != 0)
	{
		buff = malloc(sizeof(char) * (BUFFER_SIZE + 1));
		if (buff == NULL)
			return ;
		readed = (int)read(fd, buff, BUFFER_SIZE);
		if ((*stash == NULL && readed == 0) || readed == -1)
		{
			free(buff);
			return ;
		}
		buff[readed] = '\0';
		add_to_stash(stash, buff, readed, fd);
		free(buff);
	}
}

/* Extracts all characters from our stash and stores them in out line.
 * stopping after the first \n it encounters */

void	extract_line(int fd, t_list *stash, char **line)
{
	int	i;
	int	j;

	if (stash == NULL)
		return ;
	generate_line(fd, line, stash);
	if (*line == NULL)
		return ;
	j = 0;
	while (stash)
	{
		i = 0;
		if (stash->fd == fd)
		{
			while (stash->content[i])
			{
				if (stash->content[i] == '\n')
				{
					(*line)[j++] = stash->content[i];
					break ;
				}
				(*line)[j++] = stash->content[i++];
			}
		}
		stash = stash->next;
	}
	(*line)[j] = '\0';
}

/* After extracting the line that was read, we don't need those characters
 * anymore. This function clears the stash so only the characters that have
 * not been returned at the end of get_next_line remain in our static stash. */

void	clean_stash(int fd, t_list *stash)
{
	t_list	*last;
	t_list	*clean_node;
	t_list	*last_fd;
	int		i;
	int		j;

	clean_node = malloc(sizeof(t_list));
	if (stash == NULL || clean_node == NULL)
		return ;
	clean_node->next = NULL;
	if (clean_node->fd == NULL)
		return ;
	clean_node->fd = fd; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	last_fd = stash;
	while(last_fd)
	{
		if (last_fd->fd == fd)
			last = last->fd;
		last_fd == last_fd->next;
	}
	i = 0;
	while (last->content[i] && last->content[i] != '\n')
		i++;
	if (last->content && last->content[i] == '\n')
		i++;
	clean_node->content = malloc(sizeof(char) * ((ft_strlen(last->content) - i) + 1));
	if (clean_node->content == NULL)
		return ;
	j = 0;
	while (last->content[i])
		clean_node->content[j++] = last->content[i++];
	clean_node->content[j] = '\0';
	free_stash(fd, stash);
	last = ft_lst_get_last(stash);
	last->next = clean_node;
}
