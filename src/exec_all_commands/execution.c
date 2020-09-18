/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matrus <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 07:35:49 by matrus            #+#    #+#             */
/*   Updated: 2020/09/17 07:35:53 by matrus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec_all_commands.h"
#include "get_next_line.h"
#include <stdlib.h>
#include <fcntl.h>

typedef struct	s_exec_info
{
	int		fd_prev;
	int		fd_pipe[2];
	pid_t	pid;
	int		status;
}				t_exec_info;

//static void		print_execution_result(int fd_from, int fd_to)
//{
//	int		gnl_status;
//	char	*line;
//
//	while ((gnl_status = get_next_line(fd_from, &line)) >= 0)
//	{
//		write(fd_to, line, ft_strlen(line));
//		write(fd_to, "\n", *line ? 1 : 0);
//		free(line);
//		if (!gnl_status)
//			break ;
//	}
//}

static int		exec_extern(t_exec_info *inf, t_command *cmd, t_env *env)
{
	int	status;

	inf->pid = fork();
	if (inf->pid == 0)
	{
		// Child here

		// Make redirects in here. If redirect, pipeline is ignored.
		dup2(inf->fd_prev, 0);
		// Make redirects out here. If redirect, pipeline is ignored.
		dup2(inf->fd_pipe[1], 1);
		// Exec smth
		status = execve(cmd->cmd_name, cmd->args, env->transfer_control(env));
		// If Error -> write smth;
		ft_putendl_fd("Oops! Bad execution:(", 1);	// TODO: think about error and fd 2
		exit(status);
	}
	else if (inf->pid < 0)
	{
		// Fork error here
		ft_putendl_fd("Forking error", 1); // TODO: error fd 2
		return (-1);
	}
	else
	{
		// Parent here
		waitpid(inf->pid, &status, WUNTRACED);
	}
	return (WEXITSTATUS(status));
}

static int		open_out_redirect_if_exist(const char *filename, int is_double)
{
	int	fd;
	int	flags;

	if (!filename)
		return (1);
	flags = O_WRONLY | (is_double ? O_APPEND : O_TRUNC);
	fd = open(filename, flags);
	return (fd < 0 ? 1 : fd);
}

static void		close_and_set_ft_prev(int *fd_prev, char **redirects_tab)
{
	char	*filename;
	int		fd;

	filename = NULL;
	while (*redirects_tab)
		filename = *redirects_tab++;
	if (filename)
	{
		if (*fd_prev > 0)
			close(*fd_prev);
		fd = open(filename, O_RDONLY);
		*fd_prev = (fd < 0) ? 0 : fd;
	}
}

/*
**	function tries to create a pipe
**	then select which command to exec (builtin / extern)
**	executed functions write their output into a PIPE
**	PIPE.OUT saved till next execute, next execute will read from that fd
**	if separator is not a pipeline then outputs goes to redirections or to fd=1
*/

int				execute_command(t_func_ptr builtin, t_command *cmd, t_env *env)
{
	static t_exec_info	exe_i;
	int					is_pipe;

	is_pipe = cmd->next_operator[0] == '|' && cmd->next_operator[1] == '\0';
	if (cmd->redir_out_last)
		is_pipe = 0;
	if (cmd->redir_in)
		close_and_set_ft_prev(&exe_i.fd_prev, cmd->redir_in);
	exe_i.fd_pipe[0] = 0;
	exe_i.fd_pipe[1] = open_out_redirect_if_exist(cmd->redir_out_last, cmd->last_is_double);
	if (is_pipe)
		if ((pipe(exe_i.fd_pipe)) < 0)
			ft_putendl_fd("pipe error", 1); // TODO: error managment ERRNO

	if (builtin)
		exe_i.status = builtin(cmd->args, exe_i.fd_prev, exe_i.fd_pipe[1], env);
	else
		exe_i.status = exec_extern(&exe_i, cmd, env);

	if (exe_i.fd_prev > 0)
		close(exe_i.fd_prev);

	if (is_pipe || exe_i.fd_pipe[1] != 1)
		close(exe_i.fd_pipe[1]);

	exe_i.fd_prev = exe_i.fd_pipe[0];

	// Make redirects out
	// If next command not pipe - reset fds;
//	if (!is_pipe)
//	{
//		exe_i.fd_prev = 0;
//		exe_i.fd_next = 1;
//	}
//	else
//		close(exe_i.fd_pipe[0]);
	return (exe_i.status);
}
