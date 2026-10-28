# escape sequences in literals
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.yacc.014
# Character literals accept the escape sequences of C character constants.
. "$PC_CORE/pc.sh"
pc_begin XCU.yacc.014
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
s : '\x41' '\n' '\t' '\\' '\'' '\101' '\a' '\"' { puts("ok"); }
  ;
EOY
cat >lexer.c <<'EOY'
#include <stdio.h>
int yyparse(void);
void yyerror(const char *s) { (void)s; }
int yylex(void)
{
	int c = getchar();

	return c == EOF ? 0 : c;
}
int main(void) { return yyparse(); }
EOY
pc_test
yacc g.y </dev/null >y.out 2>y.err || pc_fail "yacc rejected the escape sequences"
c17 -o p y.tab.c lexer.c </dev/null >cc.out 2>&1 || pc_fail "the generated parser does not compile"
printf 'A\n\t\\\047\101\007\042' | ./p >out 2>err
rc=$?
[ "$rc" -eq 0 ] && [ "$(cat out)" = ok ] || pc_fail "status $rc, output '$(cat out)', expected ok and 0"
pc_pass
