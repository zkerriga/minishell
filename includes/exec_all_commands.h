/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_all_commands.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkerriga <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/12 17:23:57 by zkerriga          #+#    #+#             */
/*   Updated: 2020/09/12 17:24:00 by zkerriga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_ALL_COMMANDS_H
# define EXEC_ALL_COMMANDS_H

# include "libft.h"
# include "environment.h"
# include "minishell.h"

typedef int	(*t_func_ptr)(char **, int, int, t_env *);

typedef struct	s_exec_info
{
	int		fd_prev;
	int		fd_out;
	int		fd_pipe[2];
	pid_t	pid;
	int		status;
}				t_exec_info;

typedef struct	s_cmdlink
{
	const char	*cmd_name;
	int			(*func)(char **, int, int, t_env *);
}				t_cmdlink;

int				exec_one_command(t_command *cmd, t_env *env);
int				execute_command(t_func_ptr builtin, t_command *cmd, t_env *env);
int				parse_stop_status(int stat);

int				is_ok_set_cmd_exec_name(t_command *name, t_env *env);
int				open_output_redirects(t_command *cmd);
int				check_input_redirects(t_command *cmd);

#endif
