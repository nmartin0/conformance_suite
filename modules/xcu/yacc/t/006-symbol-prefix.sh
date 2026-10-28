# -p symbol prefix
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.yacc.006
# -p xx renames yyparse, yylex and yyerror to xxparse, xxlex and xxerror; a
# program defining its own yyparse links with the generated code.
. "$PC_CORE/pc.sh"
pc_begin XCU.yacc.006
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v c17 >/dev/null 2>&1 || pc_unresolved "setup: c17 is not available"
cat >g.y <<'EOY'
%{
#include <stdio.h>
int xxlex(void);
void xxerror(const char *);
%}
%%
s : 'a' 'b' { puts("ab"); }
  ;
%%
int xxlex(void)
{
	int c = getchar();

	return c == EOF ? 0 : c;
}
void xxerror(const char *s) { printf("error: %s\n", s); }
EOY
cat >m.c <<'EOY'
#include <stdio.h>
int xxparse(void);
int yyparse(void) { return 42; }
int main(void) { printf("%d %d\n", xxparse(), yyparse()); return 0; }
EOY
printf 'ab' >in
pc_test
yacc -p xx g.y </dev/null >y.out 2>y.err || pc_fail "yacc -p: nonzero status"
c17 -o p m.c y.tab.c </dev/null >cc.out 2>&1 || pc_fail "the generated xx parser does not compile and link"
./p <in >out 2>err
[ "$(tr '\n' ' ' <out)" = 'ab 0 42 ' ] || pc_fail "output was '$(tr '\n' ' ' <out)'"
pc_pass
