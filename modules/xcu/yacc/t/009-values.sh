# $$, $n, %union, %type, %start and mid-rule actions
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.yacc.009
# Semantic values flow through $$ and $n with union tags, the default value
# of a rule is its first element, a mid-rule action has a value, and %start
# overrides the first rule.
. "$PC_CORE/pc.sh"
pc_begin XCU.yacc.009
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v c17 >/dev/null 2>&1 || pc_unresolved "setup: c17 is not available"
cat >g.y <<'EOY'
%{
#include <stdio.h>
void yyerror(const char *);
%}
%union { int i; char c; }
%token <i> NUM
%type <i> expr
%start list
%%
junk : NUM
     ;
list : expr '\n' { printf("%d\n", $1); }
     ;
expr : NUM
     | expr '+' { $<i>$ = $1 * 10; } NUM { $$ = $<i>3 + $4; }
     ;
%%
void yyerror(const char *s) { printf("error: %s\n", s); }
EOY
cat >lexer.c <<'EOY'
#include <stdio.h>
#include "y.tab.h"
int yyparse(void);
int yylex(void)
{
	int c;

	while ((c = getchar()) == ' ')
		;
	if (c >= '0' && c <= '9') {
		yylval.i = c - '0';
		while ((c = getchar()) >= '0' && c <= '9')
			yylval.i = yylval.i * 10 + (c - '0');
		ungetc(c, stdin);
		return NUM;
	}
	if (c == EOF)
		return 0;
	return c;
}
int main(void) { return yyparse(); }
EOY
printf '7\n1+2\n12+30\n' >in
pc_test
yacc -d g.y </dev/null >y.out 2>y.err || pc_fail "yacc -d: nonzero status"
c17 -o p y.tab.c lexer.c </dev/null >cc.out 2>&1 || pc_fail "the generated parser does not compile"
./p <in >out 2>err
[ "$?" -eq 0 ] || pc_fail "the parser exits nonzero"
[ "$(tr '\n' ' ' <out)" = '7 12 150 ' ] || pc_fail "values were '$(tr '\n' ' ' <out)', expected '7 12 150'"
pc_pass
