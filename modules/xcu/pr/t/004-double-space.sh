# -d double-spaces the output.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pr.004
. "$PC_CORE/pc.sh"
pc_begin XCU.pr.004
LC_ALL=POSIX
export LC_ALL
NL='
'
mkn() { _n=$1; _f=$2; : >"$_f" || pc_unresolved "cannot create input"; _i=1; while [ "$_i" -le "$_n" ]; do printf 'L%s\n' "$_i"; _i=$((_i+1)); done >"$_f"; }
rng() { _a=$1; _b=$2; rng_out=; while [ "$_a" -le "$_b" ]; do rng_out=$rng_out"L$_a$NL"; _a=$((_a+1)); done; }
printf 'a\nb\n' >f || pc_unresolved "cannot create input"
pc_test
got=$(pr -t -d f 2>/dev/null </dev/null; printf x)
[ "$got" = "a${NL}${NL}b${NL}${NL}x" ] || pc_fail "-d did not add a newline after every input line"
pc_pass
