# Columns truncate long lines, -w sets the width, -s uses a separator and the 512 default width.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pr.008
. "$PC_CORE/pc.sh"
pc_begin XCU.pr.008
LC_ALL=POSIX
export LC_ALL
NL='
'
mkn() { _n=$1; _f=$2; : >"$_f" || pc_unresolved "cannot create input"; _i=1; while [ "$_i" -le "$_n" ]; do printf 'L%s\n' "$_i"; _i=$((_i+1)); done >"$_f"; }
rng() { _a=$1; _b=$2; rng_out=; while [ "$_a" -le "$_b" ]; do rng_out=$rng_out"L$_a$NL"; _a=$((_a+1)); done; }
x30=xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
x60=$x30$x30
x100=$x60$x30
printf '%s\nb\n' "$x100" >f || pc_unresolved "cannot create input"
printf '%s\nc\n' "$x60" >g || pc_unresolved "cannot create input"
pc_test
got=$(pr -t -2 f 2>/dev/null </dev/null | head -n 1)
[ "${#got}" -le 72 ] || pc_fail "a line longer than a column was not truncated to the default width"
case $got in *x*) ;; *) pc_fail "the truncated line has lost its text";; esac
got=$(pr -t -2 -w 20 f 2>/dev/null </dev/null | head -n 1)
[ "${#got}" -le 20 ] || pc_fail "-w 20 did not limit the line to 20 columns"
case $got in *x*) ;; *) pc_fail "the -w 20 line has lost its text";; esac
got=$(pr -t -2 -s: g 2>/dev/null </dev/null)
[ "$got" = "${x60}:c" ] || pc_fail "-s: did not separate the columns with a colon and keep the 60-column text"
got=$(pr -t g 2>/dev/null </dev/null | head -n 1)
case $got in *"$x60") ;; *) pc_fail "single-column output was truncated";; esac
pc_pass
