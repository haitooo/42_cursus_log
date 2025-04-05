/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 10:46:34 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/31 07:08:24 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

# include "includes.h"

/* expander */
int			expand_cmds(t_status **st_head, t_var **varlist);
t_tokens	*expander(char *str, t_var **varlist);
t_tokens	*tokenizer(char *str);
t_tokens	*expand_tokens(t_tokens **tokens, t_var **varlist);
t_tokens	*check_wildcard(t_splited **splited);
t_tokens	*get_expanded_tokens(t_splited **splited);
void		replace_vars(t_splited **splited, t_var **varlist);
int			replace_home(t_tokens **tokens, t_var **varlist);
t_tokens	*remove_empty_tokens(t_tokens **tokens);
t_tokens	*get_new_tokens(char *str, t_tokens **next, t_var **varlist);
t_splited	*pre_split_tokens(char *str);
t_splited	*split_tokens(t_splited *pre_splited);
t_splited	*split_to_words(t_splited *pre_splited);
t_splited	*get_new_splited_for_plain(char *str, int len);
t_splited	*get_new_splited_for_double(char *str, int len);
int			count_words_for_plain(char *str);
int			count_words_for_double(char *str);
void		free_splited(t_splited **splited);

/* wildcard */
t_tokens	*expand_wildcard(t_splited **splited);
t_wild		*get_expanded_files(t_splited **splited, t_wild **based_files);
char		**split_dir(char *str);
t_wild		*get_files(char *name);
void		connect_to_the_end(t_wild **files, t_wild **new_files);
int			check_wild(char *str);
t_wild		*remove_filename(t_wild **files);
void		free_file(t_wild **file);
void		free_all_files(t_wild **files);
void		concat_files(char *dirname, t_wild **files);
int			count_wild_words(char *str);
void		search_same_file(t_wild **files, t_splited **splited);
void		check_dir(t_wild **file, char **words);
char		*trim_word_start(t_wild **file, t_splited **splited, char *name);
char		*trim_word_end(t_wild **file, t_splited **splited, char *name);
void		manage_flag(t_wild **files, t_splited **splited, char *name);
t_wild		*sort_files(t_wild **files);

#endif
