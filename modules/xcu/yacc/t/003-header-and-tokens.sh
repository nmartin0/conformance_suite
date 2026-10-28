# -d header file and token numbers
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.yacc.003
# -d writes y.tab.h with #define lines for the tokens; an explicit token
# number is used, other tokens are above 256 and distinct, and a %union
# gives YYSTYPE and yylval to the header.
. "$PC_CORE/pc.sh"
pc_begin XCU.yacc.003
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v c17 >/dev/null 2>&1 || pc_unresolved "setup: c17 is not available"
cat >g.y <<'EOY'
%union { int i; }
%token NUM 300
%token <i> OTHER OTHER2
%%
s : NUM OTHER OTHER2 '+'
  ;
EOY
cat >h.c <<'EOY'
#include "y.tab.h"
int yylex(void) { return 0; }
void yyerror(const char *s) { (void)s; }
int main(void)
{
	yylval.i = 3;
	if (yylval.i != 3)
		return 1;
	if (NUM != 300 || OTHER <= 256 || OTHER2 <= 256 || OTHER == OTHER2)
		return 2;
	if (OTHER == 300 || OTHER2 == 300 || '+' == OTHER)
		return 3;
	return 0;
}
EOY
pc_test
yacc -d g.y </dev/null >y.out 2>y.err || pc_fail "yacc -d: nonzero status"
[ -s y.tab.h ] || pc_fail "yacc -d wrote no y.tab.h"
grep -E '^[[:space:]]*#[[:space:]]*define[[:space:]]+NUM[[:space:]]+300' y.tab.h >/dev/null || pc_fail "no #define NUM 300 in the header"
grep -E '^[[:space:]]*#[[:space:]]*define[[:space:]]+OTHER[[:space:]]' y.tab.h >/dev/null || pc_fail "no #define OTHER in the header"
c17 -o h h.c y.tab.c </dev/null >cc.out 2>&1 || pc_fail "the header and code file do not compile together"
./h </dev/null >/dev/null 2>&1
rc=$?
[ "$rc" -eq 0 ] || pc_fail "token numbers or yylval are wrong (check $rc)"
pc_pass
