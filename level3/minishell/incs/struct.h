/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 10:37:48 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/24 11:05:53 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

# include "includes.h"

# define MAX_STACK_BRACKETS 500

typedef enum e_wtype
{
	WILD,
	SLASH,
	ELSE,
}					t_wtype;

typedef struct s_words
{
	char			*name;
	t_wtype			type;
	struct s_words	*next;
}					t_words;

typedef struct s_wild
{
	int				flag;
	int				slash;
	char			*name;
	unsigned char	type;
	struct s_wild	*next;
}					t_wild;

typedef struct s_saved
{
	int				in;
	int				out;
}					t_saved;

typedef enum e_exist
{
	NONE,
	EXIST,
}					t_exist;

typedef enum e_type
{
	WORD,
	TRUNC,
	APPEND,
	INPUT,
	HEREDOC,
	HAVE_QUOTE,
	VAR,
}					t_type;

typedef struct s_tokens
{
	char			*token;
	t_type			type;
	struct s_tokens	*next;
}					t_tokens;

typedef struct s_var
{
	char			*name;
	char			*value;
	struct s_var	*next;
}					t_var;

typedef struct s_child
{
	t_var			**varlist;
	t_tokens		**tokens;
	char			**cmds;
	char			*fullpath;
	char			**envp;
	char			**paths;
	char			*tmpfile;
}					t_child;

typedef struct s_status
{
	struct s_status	*previous;
	char			*cmds;
	t_tokens		*token;
	pid_t			pid;
	pid_t			input_pipefd;
	pid_t			output_pipefd;
	int				has_brackets;
	int				has_or;
	int				has_and;
	int				has_and_single;
	int				has_semicolon;
	int				is_builtin;
	struct s_status	*next;
}					t_status;

typedef struct s_brackets
{
	int				top;
	int				data[MAX_STACK_BRACKETS + 1];
	int				pair[MAX_STACK_BRACKETS + 1][2];
	int				countof_pair;
	int				location;
}					t_brackets;

typedef struct s_parser
{
	int				i;
	char			*cmds;
	t_var			**var;
}					t_parser;

typedef struct s_last_process
{
	pid_t			last_pid;
	int				result;
	int				count_forked;
	char			*input;
}					t_lp;

#endif
