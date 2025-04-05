/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 10:43:45 by tssaito           #+#    #+#             */
/*   Updated: 2025/04/04 01:26:50 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "includes.h"

size_t		ft_strlen(const char *s);
int			ft_strlen_sep(const char *str, const char c);
int			ft_strcmp(const char *s1, const char *s2);
int			ft_strncmp(const char *s1, const char *s2, int n);
size_t		ft_strncpy(char *dest, const char *src, size_t size);
char		*ft_strdup(const char *s);
char		**ft_split(const char *str, char c);
char		**ft_split_sep(const char *str, char **sep, int length);
int			count_words(const char *str, char c);
int			count_sep(const char *str, char **sep, int length, int count);
char		*ft_substr(const char *s, unsigned int start, size_t len);
void		ft_strcpy(char *new_str, char *str);
int			ft_atoi(const char *str);
size_t		ft_strlcat(char *dst, const char *src, size_t dsize);
size_t		ft_strlcpy(char *dst, const char *src, size_t dsize);
int			ft_isspace(char c);
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_strjoin_three(char const *s1, char const *s2, char const *s3);
char		*ft_strndup(const char *s, int n);
void		ft_putendl_fd(char *s, int fd);
void		ft_putstr_fd(char *s, int fd);
char		*ft_itoa(int n);
void		*ft_memcpy(void *dest, const void *src, size_t n);
int			ft_isalnum(int c);
int			ft_isdigit(int c);
int			ft_islower(int c);
int			ft_isupper(int c);
int			ft_isalpha(int c);
int			has_char(char *str, char key);
int			update_exit_code(int exit_code, t_var **varlist);
void		update_last_token(t_status *st, t_var **varlist);
int			ft_atoi_exit(const char *str);
int			get_exit_status(t_var **varlist);
char		*get_path(t_var **varlist, char *path);
char		*trim_edges_space(const char *str);
char		*trim_spaces(char *str);
char		*add_char(char *cmds, char c, int *i, t_var **var);

/* free_utils */
void		free_strs(char **strs);
void		free_tokens(t_tokens **tokens);
void		free_one_token(t_tokens **tokens, t_tokens *rm_token);
void		free_words(char **words, int size);
void		free_lst_status(t_status *st_head, t_status *st);
void		free_lst_status_(t_status *st_head, char *cmds, char *heredoc);
void		frees(t_status *st_head, t_var **varlist);

/* error_utils */
void		*error_add_char(int *i, char *cmds);
int			error_handle_brackets(int error_num, t_var **var);
int			error_node(int error_num);
int			error_pipe(int error_num, t_var **var, int ope, char *cmds);

/* handle_node */
int			add_operator_node(const char *op, char **cmds, t_status **st_head,
				t_parser *ps);
t_status	*ft_new_node(const char *cmds, int has_brackets);
void		ft_add_back_node(t_status **head, t_status *new_node);
void		ft_remove_node(t_status **head, t_status *node);
int			add_command_node(char **cmds, t_status **st_head, t_var **var);

/* init_utils */
void		init_ps(t_parser *ps, const char *input, t_var **var);

#endif
