# lex -t and the default lex.yy.c
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.lex.001
# lex -t writes the C program to standard output and leaves no lex.yy.c;
# without -t the program is written to lex.yy.c; with no file operand the
# source is read from standard input; the program compiles.
. "$PC_CORE/pc.sh"
pc_begin XCU.lex.001
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
[a-z]+	printf("W");
%%
int yywrap(void) { return 1; }
int main(void) { return yylex() != 0; }
EOL
pc_test
lex -t t.l </dev/null >c.out 2>lex.err || pc_fail "lex -t: nonzero status"
[ -s c.out ] || pc_fail "lex -t wrote nothing to standard output"
[ -e lex.yy.c ] && pc_fail "lex -t created lex.yy.c"
cp c.out c0.c
c17 -o scan0 c0.c </dev/null >cc.out 2>&1 || pc_fail "the program written by -t does not compile"
lex t.l </dev/null >/dev/null 2>lex.err || pc_fail "lex: nonzero status"
[ -s lex.yy.c ] || pc_fail "lex wrote no lex.yy.c"
rm -f lex.yy.c
lex <t.l >/dev/null 2>lex.err || pc_fail "lex from standard input: nonzero status"
[ -s lex.yy.c ] || pc_fail "lex with no operand wrote no lex.yy.c"
c17 -o scan lex.yy.c </dev/null >cc.out 2>&1 || pc_fail "lex.yy.c does not compile"
printf 'ab 12\n' >in
./scan <in >out 2>err
[ "$(cat out)" = 'W 12' ] || pc_fail "scanner output: $(cat out)"
pc_pass
