# -m merges files side by side with an empty name field in the header.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pr.007
. "$PC_CORE/pc.sh"
pc_begin XCU.pr.007
LC_ALL=POSIX
export LC_ALL
NL='
'
mkn() { _n=$1; _f=$2; : >"$_f" || pc_unresolved "cannot create input"; _i=1; while [ "$_i" -le "$_n" ]; do printf 'L%s\n' "$_i"; _i=$((_i+1)); done >"$_f"; }
rng() { _a=$1; _b=$2; rng_out=; while [ "$_a" -le "$_b" ]; do rng_out=$rng_out"L$_a$NL"; _a=$((_a+1)); done; }
printf 'a1\na2\n' >f1 || pc_unresolved "cannot create input"
printf 'b1\nb2\n' >f2 || pc_unresolved "cannot create input"
printf 'c1\nc2\n' >f3 || pc_unresolved "cannot create input"
row() { printf '%s\n' "$1" | sed -n "$2p"; }
pc_test
got=$(pr -t -m f1 f2 2>/dev/null </dev/null)
set -- $(row "$got" 1)
[ "$#" -eq 2 ] && [ "$1" = a1 ] && [ "$2" = b1 ] || pc_fail "-m first row is not a1 and b1"
set -- $(row "$got" 2)
[ "$#" -eq 2 ] && [ "$1" = a2 ] && [ "$2" = b2 ] || pc_fail "-m second row is not a2 and b2"
got=$(pr -t -m f1 f2 f3 2>/dev/null </dev/null)
set -- $(row "$got" 2)
[ "$#" -eq 3 ] && [ "$1" = a2 ] && [ "$2" = b2 ] && [ "$3" = c2 ] || pc_fail "-m with three files did not give three columns"
got=$(pr -m -l 15 f1 f2 2>/dev/null </dev/null; printf x)
case $got in "$NL$NL"*"  Page 1$NL$NL${NL}"*) ;; *) pc_fail "the -m header file field is not empty";; esac
pc_pass
