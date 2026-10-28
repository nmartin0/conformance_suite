# -t drops header and trailer, a small -l does the same, -l sets the page length.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pr.002
. "$PC_CORE/pc.sh"
pc_begin XCU.pr.002
LC_ALL=POSIX
export LC_ALL
NL='
'
mkn() { _n=$1; _f=$2; : >"$_f" || pc_unresolved "cannot create input"; _i=1; while [ "$_i" -le "$_n" ]; do printf 'L%s\n' "$_i"; _i=$((_i+1)); done >"$_f"; }
rng() { _a=$1; _b=$2; rng_out=; while [ "$_a" -le "$_b" ]; do rng_out=$rng_out"L$_a$NL"; _a=$((_a+1)); done; }
mkn 30 f
pc_test
got=$(pr -t f 2>/dev/null </dev/null; printf x)
[ "$got" = "$(cat f; printf x)" ] || pc_fail "-t did not give the input back unchanged"
got=$(pr -l 10 f 2>/dev/null </dev/null; printf x)
[ "$got" = "$(cat f; printf x)" ] || pc_fail "a page length not above the header plus trailer did not suppress them"
got=$(TZ=UTC0 pr -l 15 f 2>/dev/null </dev/null; printf x)
case $got in
"$NL$NL"*" f Page 1$NL$NL${NL}L1${NL}L2${NL}L3${NL}L4${NL}L5$NL$NL$NL$NL$NL$NL$NL"*" f Page 2$NL$NL${NL}L6${NL}L7${NL}L8${NL}L9${NL}L10$NL$NL$NL$NL$NL$NL"*) ;;
*) pc_fail "-l 15 did not give 5-line page bodies between headers and trailers";;
esac
pc_pass
