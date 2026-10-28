# quoting, escapes, intervals, anchors, classes
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.lex.007
# Double-quoted text is literal, octal and hex escapes give characters, an
# interval applies to a substituted name, ^ and $ anchor, character classes
# and quoted blanks work.
. "$PC_CORE/pc.sh"
pc_begin XCU.lex.007
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v c17 >/dev/null 2>&1 || pc_unresolved "setup: c17 is not available"
cat >t.l <<'EOL'
D [0-9]
L [a-z]
%{
#include <stdio.h>
static int cnt = 0;
%}
%%
"a.b"	printf("(lit)");
a\.c	printf("(esc)");
\x41\102	printf("(AB)");
{L}{D}{2,3}	printf("(LD:%s)", yytext);
^#.*	printf("(hash)");
end$	printf("(end)");
[[:upper:]]+	printf("(U)");
"two words"	printf("(tw)");
\n	{ cnt++; putchar('\n'); }
%%
int yywrap(void) { return 1; }
int main(void) { yylex(); printf("lines=%d\n", cnt); return 0; }
EOL
printf 'a.b axb a.c\nAB x12 y1234 z123\n#comment end\nthe end\nXYZ two words\n' >in
pc_test
lex t.l </dev/null >lex.out 2>lex.err || pc_fail "lex: nonzero status"
[ -s lex.yy.c ] || pc_fail "lex wrote no lex.yy.c"
c17 -o scan lex.yy.c </dev/null >cc.out 2>&1 || pc_fail "the generated program does not compile"
./scan <in >out 2>err
[ "$?" -eq 0 ] || pc_fail "the generated program exits nonzero"
printf '(lit) axb (esc)\n(AB) (LD:x12) (LD:y123)4 (LD:z123)\n(hash)\nthe (end)\n(U) (tw)\nlines=5\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "scanner output differs: $(cat out)"
pc_pass
