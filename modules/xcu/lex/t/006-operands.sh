# file operands are concatenated and - is standard input
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.lex.006
# Several file operands form one lex program, a - operand is standard input,
# and -t works with them.
. "$PC_CORE/pc.sh"
pc_begin XCU.lex.006
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v c17 >/dev/null 2>&1 || pc_unresolved "setup: c17 is not available"
printf 'D [0-9]\n%%%%\n' >a.l
printf '{D}+\tprintf("N");\n' >mid.l
cat >c.l <<'EOL'
%%
int yywrap(void) { return 1; }
int main(void) { return yylex() != 0; }
EOL
printf 'a1 22\n' >in
pc_test
lex a.l - c.l <mid.l >lex.out 2>lex.err || pc_fail "lex with three operands: nonzero status"
[ -s lex.yy.c ] || pc_fail "lex wrote no lex.yy.c"
c17 -o scan lex.yy.c </dev/null >cc.out 2>&1 || pc_fail "the generated program does not compile"
./scan <in >out 2>err
[ "$(cat out)" = 'aN N' ] || pc_fail "scanner output: $(cat out)"
rm -f lex.yy.c
lex -t a.l - c.l <mid.l >c.out 2>lex.err || pc_fail "lex -t with a - operand: nonzero status"
[ -s c.out ] || pc_fail "lex -t wrote nothing"
cp c.out c1.c
c17 -o scan1 c1.c </dev/null >cc.out 2>&1 || pc_fail "the -t program does not compile"
./scan1 <in >out 2>err
[ "$(cat out)" = 'aN N' ] || pc_fail "the -t program output: $(cat out)"
pc_pass
