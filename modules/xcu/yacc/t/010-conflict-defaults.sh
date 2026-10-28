# default resolution of conflicts
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.yacc.010
# Conflicts are reported on standard error with a zero exit status; a
# shift/reduce conflict resolves to shift and a reduce/reduce conflict to
# the earlier rule.
. "$PC_CORE/pc.sh"
pc_begin XCU.yacc.010
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
s : a
  | b
  ;
a : 'x' { printf("a"); }
  ;
b : 'x' { printf("b"); }
  ;
%%
int yylex(void)
{
	int c = getchar();

	return c == EOF ? 0 : c;
}
void yyerror(const char *s) { (void)s; }
int main(void) { int rc = yyparse(); putchar(10); return rc; }
EOY
cat >h.y <<'EOY'
%{
#include <stdio.h>
int yylex(void);
void yyerror(const char *);
%}
%%
s : 'i' s { printf("if"); }
  | 'i' s 'e' s { printf("ifelse"); }
  | 'z'
  ;
%%
int yylex(void)
{
	int c = getchar();

	return c == EOF ? 0 : c;
}
void yyerror(const char *s) { (void)s; }
int main(void) { int rc = yyparse(); putchar(10); return rc; }
EOY
pc_test
yacc g.y </dev/null >y.out 2>y.err || pc_fail "yacc with a reduce/reduce conflict: nonzero status"
[ -s y.err ] || pc_fail "no conflict report on standard error"
c17 -o p y.tab.c </dev/null >cc.out 2>&1 || pc_fail "the generated parser does not compile"
printf 'x' >in
./p <in >out 2>err
[ "$(cat out)" = a ] || pc_fail "reduce/reduce chose '$(cat out)', expected the earlier rule a"
rm -f y.tab.c y.err
yacc h.y </dev/null >y.out 2>y.err || pc_fail "yacc with a shift/reduce conflict: nonzero status"
[ -s y.err ] || pc_fail "no conflict report for the shift/reduce conflict"
c17 -o q y.tab.c </dev/null >cc.out 2>&1 || pc_fail "the second parser does not compile"
printf 'iizez' >in2
./q <in2 >out2 2>err
[ "$(cat out2)" = ifelseif ] || pc_fail "shift/reduce gave '$(cat out2)', expected ifelseif (shift preferred)"
pc_pass
