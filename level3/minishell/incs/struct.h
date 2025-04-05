/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 10:37:48 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/31 11:37:39 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

# include "includes.h"

# define MAX_STACK_BRACKETS 500

typedef enum e_stype
{
	PLAIN,
	WILDS,
	PLAINVAR,
	QUOTEVAR,
	SINGLE,
	DOUBLE,
}						t_stype;

typedef struct s_splited
{
	char				*str;
	t_stype				type;
	struct s_splited	*next;
}						t_splited;

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

typedef enum r_type
{
	IS_OR,
	IS_ANDAND,
	IS_AND,
	IS_SEMI,
	IS_PIPE,
	IS_CMD,
}					t_ope;

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
	int				saved;
	int				has_brackets;
	int				has_or;
	int				has_and;
	int				has_and_single;
	int				has_semicolon;
	int				is_builtin;
	char			*heredoc;
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
	int				j;
	char			*cmds;
	const char		*input;
	t_var			**var;
}					t_parser;

typedef struct s_last_process
{
	pid_t			last_pid;
	int				result;
	int				count_forked;
	char			*heredoc;
	char			*heredoc_tmp;
	char			*input;
}					t_lp;

typedef struct s_heredoc
{
	int			filefd;
	char		*file;
}				t_heredoc;

#endif
