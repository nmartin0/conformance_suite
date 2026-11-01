# -m merges files side by side with an empty name field in the header.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pr.007
# MUTANT    pr.m_ignored
# MUTANT    pr.m_max2
# MUTANT    pr.m_max8
# MUTANT    pr.m_swapped
# MUTANT    pr.m_nosep
# MUTANT    pr.empty_name_filled
# VARIANT   pr.pad_to_width
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
for _p in d:4 e:5 f:6 g:7 h:8 i:9; do printf '%s1\n%s2\n' "${_p%:*}" "${_p%:*}" >"f${_p#*:}" || pc_unresolved "cannot create input"; done
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
got=$(pr -t -m f1 f2 f3 f4 f5 f6 f7 f8 f9 2>/dev/null </dev/null)
set -- $(row "$got" 2)
[ "$#" -eq 9 ] && [ "$1" = a2 ] && [ "$2" = b2 ] && [ "$3" = c2 ] && [ "$4" = d2 ] && [ "$5" = e2 ] && [ "$6" = f2 ] && [ "$7" = g2 ] && [ "$8" = h2 ] && [ "$9" = i2 ] || pc_fail "-m with nine files did not give nine columns"
pc_pass
