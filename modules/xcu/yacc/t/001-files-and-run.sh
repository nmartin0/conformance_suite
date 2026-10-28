# default output files and a working parser
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.yacc.001
# Without options yacc writes y.tab.c, not y.tab.h or y.output; the code
# file compiles, defines YYEOF as 0 and YYEMPTY as a negative number, and
# the parser evaluates input.
. "$PC_CORE/pc.sh"
pc_begin XCU.yacc.001
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v c17 >/dev/null 2>&1 || pc_unresolved "setup: c17 is not available"
cat >g.y <<'EOY'
%{
#include <stdio.h>
int yylex(void);
void yyerror(const char *);
%}
%left '+'
%left '*'
%%
input : /* empty */
      | input line
      ;
line : expr '\n' { if (YYEOF != 0 || YYEMPTY >= 0) puts("bad macros"); printf("%d\n", $1); }
     ;
expr : digit
     | expr '+' expr { $$ = $1 + $3; }
     | expr '*' expr { $$ = $1 * $3; }
     | '(' expr ')' { $$ = $2; }
     ;
digit : '0' { $$ = 0; } | '1' { $$ = 1; } | '2' { $$ = 2; } | '3' { $$ = 3; } | '4' { $$ = 4; } | '5' { $$ = 5; } | '6' { $$ = 6; } | '7' { $$ = 7; } | '8' { $$ = 8; } | '9' { $$ = 9; }
     ;
%%
int yylex(void)
{
	int c = getchar();

	return c == EOF ? 0 : c;
}
void yyerror(const char *s) { printf("error: %s\n", s); }
int main(void) { return yyparse(); }
EOY
printf '1+2*3\n(1+2)*3\n' >in
pc_test
yacc g.y </dev/null >y.out 2>y.err || pc_fail "yacc: nonzero status"
[ -s y.tab.c ] || pc_fail "yacc wrote no y.tab.c"
c17 -o p y.tab.c </dev/null >cc.out 2>&1 || pc_fail "the generated parser does not compile"
[ -e y.tab.h ] && pc_fail "y.tab.h written without -d"
[ -e y.output ] && pc_fail "y.output written without -v"
./p <in >out 2>err
[ "$?" -eq 0 ] || pc_fail "the parser exits nonzero"
[ "$(tr '\n' ' ' <out)" = '7 9 ' ] || pc_fail "the parser printed '$(cat out)'"
pc_pass
