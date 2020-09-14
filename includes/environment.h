/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkerriga <zkerriga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/14 16:35:37 by zkerriga          #+#    #+#             */
/*   Updated: 2020/09/14 16:35:59 by zkerriga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRONMENT_H
# define ENVIRONMENT_H

# include "libft.h"

/*
** This class is intended for working with environment data.
** environment_new() - Returns an object of the class
**
**
** Methods:
** add - Adds a key and value. Returns 1 if an error, otherwise returns 0.
**
** remove - Deletes the value and key by key.
**
** print - Prints all keys and their values to the passed descriptor.
**
** get_value - Returns the value by key. If there is no value, returns NULL.
**     NULL is also returned in case of an error (you need to check errno).
**
** transfer_control - Passes control over the environment data array.
**     Use only for passing to execve()!
**
** del - The destructor for the object. Completely clears the allocated memory.
*/

typedef struct	s_env
{
	int			(*add)(struct s_env *self, char *key, char *value);
	void		(*remove)(struct s_env *self, char *key);
	void		(*print)(struct s_env *self, int fd);
	char		*(*get_value)(struct s_env *self, char *key);
	char		**(*transfer_control)(struct s_env *self);
	void		(*del)(struct s_env *self);
}				t_env;

t_env			*environment_new(char **envp);

#endif