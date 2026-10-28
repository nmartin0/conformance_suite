# the yacc library
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.yacc.017
# Linking with -l y supplies main(), which calls yyparse, and yyerror, which
# writes its argument and a newline to standard error.
. "$PC_CORE/pc.sh"
pc_begin XCU.yacc.017
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v c17 >/dev/null 2>&1 || pc_unresolved "setup: c17 is not available"
cat >g.y <<'EOY'
%{
int yylex(void);
void yyerror(const char *);
%}
%%
s : 'a'
  ;
EOY
cat >lexer.c <<'EOY'
#include <stdio.h>
int yylex(void)
{
	int c = getchar();

	return c == EOF ? 0 : c;
}
EOY
pc_test
yacc g.y </dev/null >y.out 2>y.err || pc_fail "yacc rejected the grammar"
c17 -o p y.tab.c lexer.c -l y </dev/null >cc.out 2>&1 || pc_fail "cannot link with -l y"
printf 'a' | ./p >out1 2>err1
grep 'syntax error' err1 >/dev/null && pc_fail "a valid input gave a syntax error: $(cat err1)"
printf 'b' | ./p >out2 2>err2
grep 'syntax error' err2 >/dev/null || pc_fail "the library yyerror wrote '$(cat err2)' for a syntax error"
grep 'syntax error' out2 >/dev/null && pc_fail "the library yyerror wrote to standard output"
pc_pass
