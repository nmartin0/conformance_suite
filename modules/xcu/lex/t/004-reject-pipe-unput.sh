# REJECT, the | action and unput
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.lex.004
# REJECT executes the next best rule for the same input, an action of |
# shares the next rule action, and unput returns a character to the input.
. "$PC_CORE/pc.sh"
pc_begin XCU.lex.004
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v c17 >/dev/null 2>&1 || pc_unresolved "setup: c17 is not available"
cat >t.l <<'EOL'
%{
#include <stdio.h>
%}
%%
xyz	{ printf("[xyz]"); REJECT; }
xy	printf("[xy]");
p1	|
p2	printf("P");
U	{ unput('Z'); }
Z	printf("<z>");
\n	ECHO;
%%
int yywrap(void) { return 1; }
int main(void) { return yylex() != 0; }
EOL
printf 'xyz\np1p2\nU\n' >in
pc_test
lex t.l </dev/null >lex.out 2>lex.err || pc_fail "lex: nonzero status"
[ -s lex.yy.c ] || pc_fail "lex wrote no lex.yy.c"
c17 -o scan lex.yy.c </dev/null >cc.out 2>&1 || pc_fail "the generated program does not compile"
./scan <in >out 2>err
[ "$?" -eq 0 ] || pc_fail "the generated program exits nonzero"
printf '[xyz][xy]z\nPP\n<z>\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "scanner output differs: $(cat out)"
pc_pass
