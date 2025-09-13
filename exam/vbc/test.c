#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

static void skip_ws(const char *s, size_t *i) {
    while (isspace((unsigned char)s[*i])) (*i)++;
}

static int parse_expr(const char *s, size_t *i, int *err); // 先行宣言

// factor := DIGIT | '(' expr ')'
static int parse_factor(const char *s, size_t *i, int *err) {
    skip_ws(s, i);
    char c = s[*i];

    if (isdigit((unsigned char)c)) {
        // 1桁のみ許可（連続数字は構文エラー）
        int v = c - '0';
        (*i)++;
        if (isdigit((unsigned char)s[*i])) { *err = 1; return 0; }
        return v;
    }
    if (c == '(') {
        (*i)++; // '('
        int v = parse_expr(s, i, err);
        if (*err) return 0;
        skip_ws(s, i);
        if (s[*i] != ')') { *err = 1; return 0; }
        (*i)++; // ')'
        return v;
    }
    *err = 1; // 期待外
    return 0;
}

// term := factor { '*' factor }*
static int parse_term(const char *s, size_t *i, int *err) {
    int val = parse_factor(s, i, err);
    if (*err) return 0;

    for (;;) {
        skip_ws(s, i);
        if (s[*i] != '*') break;
        (*i)++; // '*'
        int rhs = parse_factor(s, i, err);
        if (*err) return 0;
        long long tmp = (long long)val * (long long)rhs;
        val = (int)tmp;
    }
    return val;
}

// expr := term { '+' term }*
static int parse_expr(const char *s, size_t *i, int *err) {
    int val = parse_term(s, i, err);
    if (*err) return 0;

    for (;;) {
        skip_ws(s, i);
        if (s[*i] != '+') break;
        (*i)++; // '+'
        int rhs = parse_term(s, i, err);
        if (*err) return 0;
        long long tmp = (long long)val + (long long)rhs;
        val = (int)tmp;
    }
    return val;
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return (1);
	const char *s = argv[1];
    size_t i = 0;
    int err = 0;

    // 先頭が演算子/右括弧はNG、空文字もNG（下のパーサで検出される）
    int result = parse_expr(s, &i, &err);
    if (!err) {
        skip_ws(s, &i);
        // 入力をすべて消費していない、または末尾が演算子相当の並びならエラー
        if (s[i] != '\0') err = 1;
    }

    if (err) return 1;
    printf("%d\n", result);
    return 0;
}
