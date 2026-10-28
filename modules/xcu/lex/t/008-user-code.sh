# copied code, rule-section prologue and table sizes
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.lex.008
# Code in %{ %} and indented lines of the definitions is copied, indented
# code before the first rule runs on each entry to yylex, user subroutines
# are copied, and table size declarations are accepted.
. "$PC_CORE/pc.sh"
pc_begin XCU.lex.008
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v c17 >/dev/null 2>&1 || pc_unresolved "setup: c17 is not available"
cat >t.l <<'EOL'
%{
#include <stdio.h>
static int cnt = 0;
%}
	static int extra = 5;
%p 3000
%n 600
%%
	int local = 7;
a	{ cnt += local; }
\n	;
%%
int yywrap(void) { return 1; }
int main(void) { yylex(); printf("cnt=%d extra=%d\n", cnt, extra); return 0; }
EOL
printf 'aaa\n' >in
pc_test
lex t.l </dev/null >lex.out 2>lex.err || pc_fail "lex: nonzero status"
[ -s lex.yy.c ] || pc_fail "lex wrote no lex.yy.c"
c17 -o scan lex.yy.c </dev/null >cc.out 2>&1 || pc_fail "the generated program does not compile"
./scan <in >out 2>err
[ "$?" -eq 0 ] || pc_fail "the generated program exits nonzero"
printf 'cnt=21 extra=5\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "scanner output differs: $(cat out)"
pc_pass
