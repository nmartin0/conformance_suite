# end of input and the endmarker
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.yacc.011
# A yylex that returns a negative value at end of input ends the parse
# successfully; end of input in the middle of a construct is a syntax error.
. "$PC_CORE/pc.sh"
pc_begin XCU.yacc.011
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v c17 >/dev/null 2>&1 || pc_unresolved "setup: c17 is not available"
cat >g.y <<'EOY'
%{
#include <stdio.h>
static int pos;
static const int toks[] = { 'a', 'b', -1 };
static const int toks2[] = { 'a', -1 };
static int which;
int yylex(void);
void yyerror(const char *);
%}
%%
s : 'a' 'b' { puts("done"); }
  ;
%%
int yylex(void)
{
	const int *t = which ? toks2 : toks;

	return t[pos++];
}
void yyerror(const char *s) { printf("error: %s\n", s); }
int main(int argc, char **argv)
{
	which = argc > 1;
	return yyparse();
}
EOY
pc_test
yacc g.y </dev/null >y.out 2>y.err || pc_fail "yacc: nonzero status"
[ -s y.tab.c ] || pc_fail "yacc wrote no y.tab.c"
c17 -o p y.tab.c </dev/null >cc.out 2>&1 || pc_fail "the generated parser does not compile"
./p </dev/null >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "a negative end-of-input value did not end the parse successfully"
[ "$(cat out)" = done ] || pc_fail "the action did not run: '$(cat out)'"
./p x </dev/null >out2 2>err
rc=$?
[ "$rc" -ne 0 ] || pc_fail "end of input in the middle of a rule was accepted"
grep error out2 >/dev/null || pc_fail "no syntax error was reported at the premature end of input"
pc_pass
