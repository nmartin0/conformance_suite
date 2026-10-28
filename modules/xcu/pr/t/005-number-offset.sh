# -n numbers lines (width, character) and -o indents output.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pr.005
. "$PC_CORE/pc.sh"
pc_begin XCU.pr.005
LC_ALL=POSIX
export LC_ALL
NL='
'
mkn() { _n=$1; _f=$2; : >"$_f" || pc_unresolved "cannot create input"; _i=1; while [ "$_i" -le "$_n" ]; do printf 'L%s\n' "$_i"; _i=$((_i+1)); done >"$_f"; }
rng() { _a=$1; _b=$2; rng_out=; while [ "$_a" -le "$_b" ]; do rng_out=$rng_out"L$_a$NL"; _a=$((_a+1)); done; }
printf 'a\nb\n' >f || pc_unresolved "cannot create input"
TAB=$(printf '\t')
pc_test
got=$(pr -t -n f 2>/dev/null </dev/null; printf x)
[ "$got" = "    1${TAB}a${NL}    2${TAB}b${NL}x" ] || pc_fail "-n did not number lines with 5 digits and a tab"
got=$(pr -t -n3 f 2>/dev/null </dev/null; printf x)
[ "$got" = "  1${TAB}a${NL}  2${TAB}b${NL}x" ] || pc_fail "-n3 did not use a 3-digit number"
got=$(pr -t -n:3 f 2>/dev/null </dev/null; printf x)
[ "$got" = "  1:a${NL}  2:b${NL}x" ] || pc_fail "-n:3 did not separate the number with the given character"
got=$(pr -t -o 4 f 2>/dev/null </dev/null; printf x)
[ "$got" = "    a${NL}    b${NL}x" ] || pc_fail "-o 4 did not precede each line with 4 spaces"
pc_pass
