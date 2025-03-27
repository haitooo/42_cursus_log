/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 10:46:34 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/26 20:30:11 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

# include "includes.h"

/* expander */
t_tokens	*expander(char *str, t_var **varlist);
t_tokens	*tokenizer(char *str);
int			remove_quotes_and_split_tokens(t_tokens **tokens, t_type type);
t_tokens	*remove_empty_tokens(t_tokens **tokens);

/* replace vars */
int			replace_vars(t_tokens **tokens, t_var **varlist);
char		*count_plaintext_size(char *str);
int			count_words_and_vars(char *str);
char		**get_words(char *str, t_var **varlist, int words_size);
char		**split_words(char *str, t_type flag);
int			is_var(char *str);
char		*dup_var(char *str, char **words, t_type type, t_var **varlist);
char		*dup_plain_text(char *str, char **words, int *i);
char		*dup_singlequot_text(char *str, char **words, int *i);
char		*dup_plaintext_in_doublequote(char *str, char **words, int i);
char		*dup_doublequot_text(char *str, char **words, int *i,
				t_var **varlist);

/* wildcard */
void		expand_wildcard(t_tokens **tokens);
t_tokens	*open_wildcard(t_tokens **tokens);
char		**split_dir(char *str);
t_wild		*get_files(char *name);
void		connect_to_the_end(t_wild **files, t_wild **new_files);
int			check_wild(char *str);
t_words		*split_wildcards(char *str);
t_wild		*remove_filename(t_wild **files);
void		free_file(t_wild **file);
void		free_all_files(t_wild **files);
void		concat_files(char *dirname, t_wild **files);
int			count_wild_words(char *str);
void		search_same_file(t_wild **files, char *str);
t_wild		*get_expanded_files(char **dirs, t_wild **based_files, int index);
void		check_dir(t_wild **file, char **words);
char		*trim_word_start(t_wild **file, t_words **words, char *name);
char		*trim_word_end(t_wild **file, t_words **words, char *name);
void		manage_flag(t_wild **files, t_words **words, char *name);
t_wild		*sort_files(t_wild **files);

#endif
