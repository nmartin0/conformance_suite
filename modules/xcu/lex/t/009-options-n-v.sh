# -n and -v with -t
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.lex.009
# -n is accepted and the program is still written; with -t and -v the
# statistics go to standard error so standard output is still a valid C
# program.
. "$PC_CORE/pc.sh"
pc_begin XCU.lex.009
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
lex -n t.l </dev/null >lex.out 2>lex.err || pc_fail "lex -n: nonzero status"
[ -s lex.yy.c ] || pc_fail "lex -n wrote no lex.yy.c"
rm -f lex.yy.c
lex -t -v t.l </dev/null >c.out 2>lex.err || pc_fail "lex -t -v: nonzero status"
[ -s c.out ] || pc_fail "lex -t -v wrote nothing to standard output"
cp c.out c2.c
c17 -o scan2 c2.c </dev/null >cc.out 2>&1 || pc_fail "standard output of lex -t -v is not a compilable program"
lex -v t.l </dev/null >/dev/null 2>&1 || pc_fail "lex -v: nonzero status"
pc_pass
