# -e expands input tabs and -i replaces spaces by tabs.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pr.009
. "$PC_CORE/pc.sh"
pc_begin XCU.pr.009
LC_ALL=POSIX
export LC_ALL
NL='
'
mkn() { _n=$1; _f=$2; : >"$_f" || pc_unresolved "cannot create input"; _i=1; while [ "$_i" -le "$_n" ]; do printf 'L%s\n' "$_i"; _i=$((_i+1)); done >"$_f"; }
rng() { _a=$1; _b=$2; rng_out=; while [ "$_a" -le "$_b" ]; do rng_out=$rng_out"L$_a$NL"; _a=$((_a+1)); done; }
TAB=$(printf '\t')
printf 'a\tb\n' >t1 || pc_unresolved "cannot create input"
printf 'aXb\n' >t2 || pc_unresolved "cannot create input"
printf 'a       b\n' >s1 || pc_unresolved "cannot create input"
printf 'a   b\n' >s2 || pc_unresolved "cannot create input"
pc_test
got=$(pr -t -e t1 2>/dev/null </dev/null; printf x)
[ "$got" = "a       b${NL}x" ] || pc_fail "-e did not expand a tab to column 9"
got=$(pr -t -e3 t1 2>/dev/null </dev/null; printf x)
[ "$got" = "a  b${NL}x" ] || pc_fail "-e3 did not expand a tab to column 4"
got=$(pr -t -eX t2 2>/dev/null </dev/null; printf x)
[ "$got" = "a       b${NL}x" ] || pc_fail "-eX did not treat X as the input tab"
got=$(pr -t -i s1 2>/dev/null </dev/null; printf x)
[ "$got" = "a${TAB}b${NL}x" ] || pc_fail "-i did not replace spaces reaching column 9 by a tab"
got=$(pr -t -i:4 s2 2>/dev/null </dev/null; printf x)
[ "$got" = "a:b${NL}x" ] || pc_fail "-i:4 did not replace spaces reaching column 5 by a colon"
pc_pass
