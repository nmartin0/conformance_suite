# substitutions and start conditions
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.lex.003
# A name defined in the definitions section is substituted for {name}; %s
# start conditions keep unprefixed rules active, %x conditions disable them,
# BEGIN switches and INITIAL (or 0) returns.
. "$PC_CORE/pc.sh"
pc_begin XCU.lex.003
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
D [0-9]
%s A
%x B
%%
start	BEGIN A;
<A>{D}+	printf("(num %s)", yytext);
<A>end	BEGIN B;
<B>[a-z]+	printf("(B:%s)", yytext);
<B>\n	BEGIN INITIAL;
<B>.	;
[a-z0-9]+	printf("(I:%s)", yytext);
\n	ECHO;
%%
int yywrap(void) { return 1; }
int main(void) { return yylex() != 0; }
EOL
printf 'start 45 foo end bar 123\ngo 7\n' >in
pc_test
lex t.l </dev/null >lex.out 2>lex.err || pc_fail "lex: nonzero status"
[ -s lex.yy.c ] || pc_fail "lex wrote no lex.yy.c"
c17 -o scan lex.yy.c </dev/null >cc.out 2>&1 || pc_fail "the generated program does not compile"
./scan <in >out 2>err
[ "$?" -eq 0 ] || pc_fail "the generated program exits nonzero"
printf ' (num 45) (I:foo) (B:bar)(I:go) (I:7)\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "scanner output differs: $(cat out)"
pc_pass
