# minimum table sizes
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.yacc.019
# A grammar with 120 tokens, 150 non-terminals and 210 rules is accepted and
# its parser compiles.
. "$PC_CORE/pc.sh"
pc_begin XCU.yacc.019
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v c17 >/dev/null 2>&1 || pc_unresolved "setup: c17 is not available"
awk 'BEGIN {
	print "%{\nint yylex(void);\nvoid yyerror(const char *);\n%}"
	for (i = 1; i <= 60; i++) printf "%%token T%d U%d\n", i, i
	print "%%"
	print "s : n1 ;"
	for (i = 1; i < 150; i++) {
		printf "n%d : n%d T%d\n", i, i + 1, i % 60 + 1
		if (i <= 60) printf "   | U%d\n", i
		print "   ;"
	}
	print "n150 : T1 ;"
}' </dev/null >g.y
cat >lexer.c <<'EOY'
int yyparse(void);
void yyerror(const char *s) { (void)s; }
int yylex(void) { return 0; }
int main(void) { (void)yyparse(); return 0; }
EOY
pc_test
yacc g.y </dev/null >y.out 2>y.err || pc_fail "yacc rejected a grammar within the minimum table sizes"
c17 -o p y.tab.c lexer.c </dev/null >cc.out 2>&1 || pc_fail "the generated parser does not compile"
pc_pass
