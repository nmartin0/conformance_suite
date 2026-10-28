# -F uses form-feed characters between pages; the default does not.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pr.010
. "$PC_CORE/pc.sh"
pc_begin XCU.pr.010
LC_ALL=POSIX
export LC_ALL
NL='
'
mkn() { _n=$1; _f=$2; : >"$_f" || pc_unresolved "cannot create input"; _i=1; while [ "$_i" -le "$_n" ]; do printf 'L%s\n' "$_i"; _i=$((_i+1)); done >"$_f"; }
rng() { _a=$1; _b=$2; rng_out=; while [ "$_a" -le "$_b" ]; do rng_out=$rng_out"L$_a$NL"; _a=$((_a+1)); done; }
FF=$(printf '\f')
mkn 100 f
pc_test
got=$(pr -F f 2>/dev/null </dev/null; printf x)
case $got in *"$FF"*) ;; *) pc_fail "-F output contains no form-feed";; esac
case $got in *"L1$NL"*"L56$NL"*"Page 2"*"L57$NL"*) ;; *) pc_fail "-F output lacks the headers and lines of both pages";; esac
got=$(pr f 2>/dev/null </dev/null; printf x)
case $got in *"$FF"*) pc_fail "output has a form-feed without -F";; esac
pc_pass
