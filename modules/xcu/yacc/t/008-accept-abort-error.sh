# YYACCEPT, YYABORT and YYERROR
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.yacc.008
# YYACCEPT makes yyparse return zero at once, YYABORT returns non-zero, and
# YYERROR returns non-zero without calling yyerror when there is no recovery
# rule.
. "$PC_CORE/pc.sh"
pc_begin XCU.yacc.008
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v c17 >/dev/null 2>&1 || pc_unresolved "setup: c17 is not available"
cat >g.y <<'EOY'
%{
#include <stdio.h>
static int nerr;
int yylex(void);
void yyerror(const char *);
%}
%%
s : 'a' { YYACCEPT; }
  | 'b' { YYABORT; }
  | 'c' { YYERROR; }
  | 'd' 'e'
  ;
%%
int yylex(void)
{
	int c = getchar();

	return c == EOF ? 0 : c;
}
void yyerror(const char *s) { (void)s; nerr++; }
int main(void) { int rc = yyparse(); printf("%d %d\n", rc != 0, nerr); return 0; }
EOY
pc_test
yacc g.y </dev/null >y.out 2>y.err || pc_fail "yacc: nonzero status"
[ -s y.tab.c ] || pc_fail "yacc wrote no y.tab.c"
c17 -o p y.tab.c </dev/null >cc.out 2>&1 || pc_fail "the generated parser does not compile"
for pair in 'ajunk:0 0' 'b:1 0' 'c:1 0' 'de:0 0' 'x:1 1'; do
	text=${pair%%:*}
	want=${pair#*:}
	printf '%s' "$text" >in
	./p <in >out 2>err
	[ "$(cat out)" = "$want" ] || pc_fail "input '$text' gave '$(cat out)', expected '$want'"
done
pc_pass
