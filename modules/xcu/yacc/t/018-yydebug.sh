# YYDEBUG default
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.yacc.018
# The code file defines YYDEBUG as 1 with -t and as 0 without it when it is
# not defined already, and a definition on the compile line is kept.
. "$PC_CORE/pc.sh"
pc_begin XCU.yacc.018
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v c17 >/dev/null 2>&1 || pc_unresolved "setup: c17 is not available"
cat >g.y <<'EOY'
%{
int yylex(void);
void yyerror(const char *);
int pc_flag = -1;
%}
%%
s : 'a' { pc_flag = YYDEBUG; }
  ;
EOY
cat >lexer.c <<'EOY'
#include <stdio.h>
int yyparse(void);
extern int pc_flag;
static int n;
void yyerror(const char *s) { (void)s; }
int yylex(void) { return n++ == 0 ? 'a' : 0; }
int main(void) { (void)yyparse(); printf("%d\n", pc_flag); return 0; }
EOY
pc_test
yacc -t g.y </dev/null >y.out 2>y.err || pc_fail "yacc -t failed"
c17 -o p1 y.tab.c lexer.c </dev/null >cc.out 2>&1 || pc_fail "the -t parser does not compile"
[ "$(./p1)" = 1 ] || pc_fail "with -t YYDEBUG was '$(./p1)', expected 1"
c17 -D YYDEBUG=0 -o p2 y.tab.c lexer.c </dev/null >cc.out 2>&1 || pc_fail "the parser does not compile with -D YYDEBUG=0"
[ "$(./p2)" = 0 ] || pc_fail "-D YYDEBUG=0 gave '$(./p2)', expected 0"
yacc g.y </dev/null >y.out 2>y.err || pc_fail "yacc failed"
c17 -o p3 y.tab.c lexer.c </dev/null >cc.out 2>&1 || pc_fail "the plain parser does not compile"
[ "$(./p3)" = 0 ] || pc_fail "without -t YYDEBUG was '$(./p3)', expected 0"
pc_pass
