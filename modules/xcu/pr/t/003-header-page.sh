# -h replaces the file name, +page starts at that page, standard input pages have an empty name.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pr.003
. "$PC_CORE/pc.sh"
pc_begin XCU.pr.003
LC_ALL=POSIX
export LC_ALL
NL='
'
mkn() { _n=$1; _f=$2; : >"$_f" || pc_unresolved "cannot create input"; _i=1; while [ "$_i" -le "$_n" ]; do printf 'L%s\n' "$_i"; _i=$((_i+1)); done >"$_f"; }
rng() { _a=$1; _b=$2; rng_out=; while [ "$_a" -le "$_b" ]; do rng_out=$rng_out"L$_a$NL"; _a=$((_a+1)); done; }
mkn 20 f
touch -t 202001020304 f || pc_unresolved "cannot set the modification time"
pc_test
got=$(TZ=UTC0 pr -l 15 -h 'My Title' f 2>/dev/null </dev/null; printf x)
case $got in "$NL${NL}Jan  2 03:04 2020 My Title Page 1$NL$NL$NL"*) ;; *) pc_fail "-h did not replace the file name in the header";; esac
got=$(TZ=UTC0 pr -l 15 +2 f 2>/dev/null </dev/null; printf x)
case $got in "$NL${NL}Jan  2 03:04 2020 f Page 2$NL$NL${NL}L6$NL"*) ;; *) pc_fail "+2 did not begin the output at page 2";; esac
case $got in *L5$NL*) pc_fail "+2 output still has lines of page 1";; esac
got=$(TZ=UTC0 pr -l 15 <f 2>/dev/null; printf x)
case $got in "$NL$NL"*"  Page 1$NL$NL${NL}L1$NL"*) ;; *) pc_fail "a page from standard input does not have an empty file field";; esac
pc_pass
