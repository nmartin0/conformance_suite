# error token, yyerrok and the syntax error call
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.yacc.007
# A syntax error calls yyerror with "syntax error", the error token lets the
# parser recover, no second call is made while recovering, and yyerrok
# resets the recovery state.
. "$PC_CORE/pc.sh"
pc_begin XCU.yacc.007
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
%%
input : /* empty */
      | input line
      ;
line : '\n'
     | expr '\n' { printf("ok\n"); }
     | error '\n' { yyerrok; printf("recovered\n"); }
     ;
expr : digit
     | expr '+' digit
     ;
digit : '0' | '1' | '2' | '3'
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
printf '1+2\n+\n3\n' >in
pc_test
yacc g.y </dev/null >y.out 2>y.err || pc_fail "yacc: nonzero status"
[ -s y.tab.c ] || pc_fail "yacc wrote no y.tab.c"
c17 -o p y.tab.c </dev/null >cc.out 2>&1 || pc_fail "the generated parser does not compile"
./p <in >out 2>err
[ "$?" -eq 0 ] || pc_fail "the parser exits nonzero after recovering"
printf 'ok\nerror: syntax error\nrecovered\nok\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "output differs: $(tr '\n' '|' <out)"
pc_pass
