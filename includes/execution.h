/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 13:16:21 by igngonza          #+#    #+#             */
/*   Updated: 2025/07/14 15:36:46 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "builtins.h"
# include "tokenizer.h"
# include <errno.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

# define ERR_INFILE "Infile"
# define ERR_OUTFILE "Outfile"
# define ERR_INPUT "Invalid number of arguments.\n"
# define ERR_PIPE "Pipe"
# define ERR_ENVP "Environment"
# define ERR_CMD "Command not found: "
# define ERR_HEREDOC "here_doc"

typedef int						t_pid;
typedef struct s_command_part	t_command_part;
typedef struct s_pipex
{
	int							in_fd;
	int							out_fd;
	int							here_doc;
	int							is_invalid_infile;
	int							*redir_failures;
	char						**cmd_paths;
	char						***cmd_args;
	int							cmd_count;
	int							pipe_count;
	int							*pipes;
	int							idx;
	t_pid						pid;
	t_pid						*pids;
	t_command_part				**cmd_segs;
	char						**heredoc_filenames;
	int							heredoc_interrupted;
}								t_pipex;

int								execution(t_command_part **cmd_segs,
									t_shell *shell);

void							*ft_bzero(void *s, size_t n);
void							create_pipes(t_pipex *pipex);
void							close_pipes(t_pipex *pipex);

void							execute_child_command(t_pipex *pipex,
									t_env *envp);
int								ft_strcmp(const char *s1, const char *s2);

char							*create_heredoc_filename(void);
void							process_heredoc_input(char *limiter, int fd,
									int type, t_shell *shell);
void							handle_heredoc(t_command_part *redir,
									t_pipex *pipex, t_shell *shell, int i);
void							heredoc_child(char *limiter, int type,
									char *filename, t_shell *shell);
void							heredoc_parent(char *filename, t_shell *shell,
									t_pipex *pipex, int i);

void							parse_paths(t_pipex *pipex, t_shell *shell);
void							create_child_process(t_pipex *pipex,
									t_shell *shell);

void							parent_free(t_pipex *pipex);

void							handle_error(const char *message);
void							print_error_and_exit(char *cmd, char *msg,
									int code);
void							print_exec_error_and_exit(char *cmd);
char							*join_path_cmd(char *dir, char *cmd);
void							cleanup_pipex(t_pipex *pipex);
void							safe_close_fd(int *fd);
void							handle_redirections(t_pipex *px,
									t_shell *shell);
void							handle_redirection_error(char *file);
void							set_signal_handlers(int mode);
void							free_token_matrix(t_command_part **matrix);
int								count_args(t_command_part *p);
char							**build_argv(t_command_part *p, int argc);
void							init_pipex(t_pipex *px);
void							parse_cmds_from_tokens(t_pipex *px,
									t_command_part **segs, t_shell *shell);
int								handle_empty_command(t_pipex *px,
									t_shell *shell);
int								handle_single_builtin(t_pipex *px,
									t_shell *shell);
pid_t							spawn_pipeline(t_pipex *px, t_shell *shell);
int								collect_status(pid_t last_pid);

#endif