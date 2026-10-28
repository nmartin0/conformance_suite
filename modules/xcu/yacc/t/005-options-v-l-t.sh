# -v, -l and -t
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.yacc.005
# -v writes y.output, -l leaves #line out of the code file, and -t makes the
# debugging variable yydebug available with initial value zero.
. "$PC_CORE/pc.sh"
pc_begin XCU.yacc.005
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v c17 >/dev/null 2>&1 || pc_unresolved "setup: c17 is not available"
cat >g.y <<'EOY'
%%
s : 'a' ;
EOY
cat >m.c <<'EOY'
extern int yydebug;
int yylex(void) { return 0; }
void yyerror(const char *s) { (void)s; }
int main(void) { return yydebug; }
EOY
pc_test
yacc -v g.y </dev/null >y.out 2>y.err || pc_fail "yacc -v: nonzero status"
[ -s y.output ] || pc_fail "yacc -v wrote no y.output"
rm -f y.tab.c y.output
yacc -l g.y </dev/null >y.out 2>y.err || pc_fail "yacc -l: nonzero status"
[ -s y.tab.c ] || pc_fail "yacc -l wrote no y.tab.c"
grep '#line' y.tab.c >/dev/null && pc_fail "yacc -l left #line directives in the code file"
rm -f y.tab.c
yacc -t g.y </dev/null >y.out 2>y.err || pc_fail "yacc -t: nonzero status"
c17 -o m m.c y.tab.c </dev/null >cc.out 2>&1 || pc_fail "yacc -t code does not provide yydebug"
./m </dev/null >/dev/null 2>&1 || pc_fail "yydebug is not initially zero"
pc_pass
