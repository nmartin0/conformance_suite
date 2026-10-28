# actions, copied code and int values
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.yacc.016
# Actions run as rules are recognised, may contain nested blocks and braces
# inside string literals, declarations in the percent-brace section have
# global scope, program-section code is copied, and values are int by
# default.
. "$PC_CORE/pc.sh"
pc_begin XCU.yacc.016
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
int total;
%}
%token NUM
%%
list : list item
     | item
     ;
item : NUM { { int t = $1; total += t; } printf("{%d}\n", $1); }
     ;
%%
int report(void) { return total; }
EOY
cat >lexer.c <<'EOY'
#include <stdio.h>
#include "y.tab.h"
int yyparse(void);
int report(void);
extern int yylval;
void yyerror(const char *s) { (void)s; }
int yylex(void)
{
	int c = getchar();

	if (c >= '0' && c <= '9') {
		yylval = c - '0';
		return NUM;
	}
	return c == EOF ? 0 : c;
}
int main(void)
{
	int r = yyparse();

	printf("total %d\n", report());
	return r;
}
EOY
printf '{1}\n{2}\n{3}\ntotal 6\n' >exp
pc_test
yacc -d g.y </dev/null >y.out 2>y.err || pc_fail "yacc rejected the grammar"
c17 -o p y.tab.c lexer.c </dev/null >cc.out 2>&1 || pc_fail "the generated parser does not compile"
printf '123' | ./p >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "the parser exits $rc"
cmp -s out exp || pc_fail "output was '$(tr '\n' ' ' <out)', expected '$(tr '\n' ' ' <exp)'"
pc_pass
