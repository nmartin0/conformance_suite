# free-form grammar syntax
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.yacc.013
# Comments may appear between symbols, names may hold periods, underscores
# and non-initial digits, upper and lower case names differ, bodies may be
# empty, alternatives share a left-hand side, and a file may end after the
# last rule without a second double percent.
. "$PC_CORE/pc.sh"
pc_begin XCU.yacc.013
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
%token X_1 301
%%
/* leading comment */
s    : /* c */ ab /* c2 */ AB y.z tail   { puts("ok"); }
     ;
ab   : 'a'
     ;
AB   : 'b'
     ;
y.z  : /* empty */
     ;
tail :
     | tail 'c'
     | tail X_1
     ;
EOY
cat >lexer.c <<'EOY'
#include <stdio.h>
int yyparse(void);
void yyerror(const char *s) { (void)s; }
int yylex(void)
{
	int c = getchar();

	if (c == EOF)
		return 0;
	if (c == 'z')
		return 301;
	return c;
}
int main(void) { return yyparse(); }
EOY
pc_test
yacc g.y </dev/null >y.out 2>y.err || pc_fail "yacc rejected the grammar"
c17 -o p y.tab.c lexer.c </dev/null >cc.out 2>&1 || pc_fail "the generated parser does not compile"
printf 'abcczc' | ./p >out 2>err
rc=$?
[ "$rc" -eq 0 ] && [ "$(cat out)" = ok ] || pc_fail "abcczc: status $rc, output '$(cat out)', expected ok and 0"
printf 'bb' | ./p >out 2>err
rc=$?
[ "$rc" -ne 0 ] || pc_fail "bb was accepted, so names differing in case were treated alike"
pc_pass
