# $0 and $-1 in actions
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.yacc.015
# A zero or negative number in an action refers to the stack entries left of
# the rule, both in the plain form and with a tag.
. "$PC_CORE/pc.sh"
pc_begin XCU.yacc.015
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v c17 >/dev/null 2>&1 || pc_unresolved "setup: c17 is not available"
cat >g1.y <<'EOY'
%{
#include <stdio.h>
int yylex(void);
void yyerror(const char *);
%}
%union { int i; }
%token <i> NUM
%type <i> item
%%
s : NUM NUM item { printf("%d\n", $3); }
  ;
item : NUM { $$ = $<i>-1 * 100 + $<i>0 * 10 + $1; }
  ;
EOY
cat >g2.y <<'EOY'
%{
#include <stdio.h>
int yylex(void);
void yyerror(const char *);
%}
%token NUM
%%
s : NUM NUM item { printf("%d\n", $3); }
  ;
item : NUM { $$ = $-1 * 100 + $0 * 10 + $1; }
  ;
EOY
cat >lex1.c <<'EOY'
#include <stdio.h>
#include "y.tab.h"
int yyparse(void);
void yyerror(const char *s) { (void)s; }
int yylex(void)
{
	int c = getchar();

	if (c >= '0' && c <= '9') {
		yylval.i = c - '0';
		return NUM;
	}
	return c == EOF ? 0 : c;
}
int main(void) { return yyparse(); }
EOY
sed 's/yylval\.i/yylval/' lex1.c >lex2.c
mkdir plain || pc_unresolved "cannot make a work directory"
pc_test
yacc -d g1.y </dev/null >y.out 2>y.err || pc_fail "yacc rejected the tagged left-context references"
c17 -o p1 y.tab.c lex1.c </dev/null >cc.out 2>&1 || pc_fail "the tagged parser does not compile"
printf '123' | ./p1 >out 2>err
[ "$(cat out)" = 123 ] || pc_fail "tagged \$<i>0 and \$<i>-1 gave '$(cat out)', expected 123"
cd plain
yacc -d ../g2.y </dev/null >y.out 2>y.err || pc_fail "yacc rejected \$0 and \$-1"
cp ../lex2.c lex2.c || pc_unresolved "cannot copy the lexer"
c17 -o p2 y.tab.c lex2.c </dev/null >cc.out 2>&1 || pc_fail "the plain parser does not compile"
printf '456' | ./p2 >out 2>err
[ "$(cat out)" = 456 ] || pc_fail "\$0 and \$-1 gave '$(cat out)', expected 456"
cd ..
pc_pass
