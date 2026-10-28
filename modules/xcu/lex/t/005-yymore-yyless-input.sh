# yymore, yyless and input
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.lex.005
# yymore appends the next match to yytext and adjusts yyleng, yyless keeps
# the first n characters and rescans the rest, and input reads and consumes
# the next character.
. "$PC_CORE/pc.sh"
pc_begin XCU.lex.005
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
ab	{ printf("(ab)"); yymore(); }
cd	printf("(%s:%d)", yytext, yyleng);
foo[a-z]*	{ yyless(3); printf("{%s}", yytext); }
bar	printf("<bar>");
Q	{ int c = input(); printf("[%c]", c); }
\n	ECHO;
%%
int yywrap(void) { return 1; }
int main(void) { return yylex() != 0; }
EOL
printf 'abcd\nfoobar\nQA\n' >in
pc_test
lex t.l </dev/null >lex.out 2>lex.err || pc_fail "lex: nonzero status"
[ -s lex.yy.c ] || pc_fail "lex wrote no lex.yy.c"
c17 -o scan lex.yy.c </dev/null >cc.out 2>&1 || pc_fail "the generated program does not compile"
./scan <in >out 2>err
[ "$?" -eq 0 ] || pc_fail "the generated program exits nonzero"
printf '(ab)(abcd:4)\n{foo}<bar>\n[A]\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "scanner output differs: $(cat out)"
pc_pass
