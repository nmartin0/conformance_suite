# %left, %right, %nonassoc and %prec
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.yacc.002
# Declared precedence and associativity resolve conflicts: higher precedence
# binds tighter, %left reduces, %right shifts, %nonassoc makes chained use a
# syntax error, and %prec gives a rule the precedence of a token.
. "$PC_CORE/pc.sh"
pc_begin XCU.yacc.002
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v c17 >/dev/null 2>&1 || pc_unresolved "setup: c17 is not available"
cat >g.y <<'EOY'
%{
#include <stdio.h>
int yylex(void);
void yyerror(const char *);
%}
%nonassoc '<'
%left '+' '-'
%left '*' '/'
%right UMINUS
%right '^'
%%
input : /* empty */
      | input line
      ;
line : expr '\n' { printf("%d\n", $1); }
     ;
expr : digit
     | expr '+' expr { $$ = $1 + $3; }
     | expr '-' expr { $$ = $1 - $3; }
     | expr '*' expr { $$ = $1 * $3; }
     | expr '/' expr { $$ = $1 / $3; }
     | expr '^' expr { int r = 1, i; for (i = 0; i < $3; i++) r *= $1; $$ = r; }
     | '-' expr %prec UMINUS { $$ = -$2; }
     | '(' expr ')' { $$ = $2; }
     | expr '<' expr { $$ = $1 < $3; }
     ;
digit : '0' { $$ = 0; } | '1' { $$ = 1; } | '2' { $$ = 2; } | '3' { $$ = 3; } | '4' { $$ = 4; } | '5' { $$ = 5; } | '6' { $$ = 6; } | '7' { $$ = 7; } | '8' { $$ = 8; } | '9' { $$ = 9; }
     ;
%%
int yylex(void)
{
	int c = getchar();

	return c == EOF ? 0 : c;
}
void yyerror(const char *s) { printf("error: %s\n", s); }
int main(void) { return yyparse(); }
EOY
printf '1+2*3\n(1+2)*3\n9-4-3\n8/4/2\n2^3^2\n-2+3\n-2^2\n2*3^2\n1+1<3\n' >in
printf '1<2<3\n' >in2
pc_test
yacc g.y </dev/null >y.out 2>y.err || pc_fail "yacc: nonzero status"
[ -s y.tab.c ] || pc_fail "yacc wrote no y.tab.c"
c17 -o p y.tab.c </dev/null >cc.out 2>&1 || pc_fail "the generated parser does not compile"
./p <in >out 2>err
[ "$?" -eq 0 ] || pc_fail "the parser exits nonzero"
printf '7\n9\n2\n1\n512\n1\n-4\n18\n1\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "results differ: $(tr '\n' ' ' <out)"
./p <in2 >out2 2>err
rc=$?
grep error out2 >/dev/null || pc_fail "chained %nonassoc operators were accepted"
[ "$rc" -ne 0 ] || pc_fail "chained %nonassoc operators gave a zero parse status"
pc_pass
