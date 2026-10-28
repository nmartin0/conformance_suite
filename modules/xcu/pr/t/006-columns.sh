# -column writes down columns, -a fills across.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pr.006
. "$PC_CORE/pc.sh"
pc_begin XCU.pr.006
LC_ALL=POSIX
export LC_ALL
NL='
'
mkn() { _n=$1; _f=$2; : >"$_f" || pc_unresolved "cannot create input"; _i=1; while [ "$_i" -le "$_n" ]; do printf 'L%s\n' "$_i"; _i=$((_i+1)); done >"$_f"; }
rng() { _a=$1; _b=$2; rng_out=; while [ "$_a" -le "$_b" ]; do rng_out=$rng_out"L$_a$NL"; _a=$((_a+1)); done; }
printf 'a\nb\nc\nd\n' >f || pc_unresolved "cannot create input"
row() { printf '%s\n' "$1" | sed -n "$2p"; }
pc_test
got=$(pr -t -2 f 2>/dev/null </dev/null)
set -- $(row "$got" 1)
[ "$#" -eq 2 ] && [ "$1" = a ] && [ "$2" = c ] || pc_fail "-2 first row is not a and c"
set -- $(row "$got" 2)
[ "$#" -eq 2 ] && [ "$1" = b ] && [ "$2" = d ] || pc_fail "-2 second row is not b and d"
n=$(printf '%s\n' "$got" | wc -l)
[ "$n" -eq 2 ] || pc_fail "-2 -t did not use the minimum number of lines"
got=$(pr -t -2 -a f 2>/dev/null </dev/null)
set -- $(row "$got" 1)
[ "$#" -eq 2 ] && [ "$1" = a ] && [ "$2" = b ] || pc_fail "-a first row is not a and b"
set -- $(row "$got" 2)
[ "$#" -eq 2 ] && [ "$1" = c ] && [ "$2" = d ] || pc_fail "-a second row is not c and d"
pc_pass
