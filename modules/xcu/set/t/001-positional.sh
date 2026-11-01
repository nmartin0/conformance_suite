# set with operands replaces all positional parameters and $# with them;
# earlier ones that are not replaced become unset.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.set.001
# MUTANT    set.append
# MUTANT    set.droplast
# MUTANT    set.dropfirst
# MUTANT    set.reverse
# MUTANT    set.keepold
# MUTANT    set.dropempty
# MUTANT    set.max9
# MUTANT    set.hash_stale
. "$PC_CORE/pc.sh"
pc_begin XCU.set.001
nl='
'
# pcrun: run pc.s in a fresh shell; out = its standard output, rc = its
# exit status, its standard error is left in pc.err.
pcrun() {
	out=$(${PC_SH:-sh} pc.s 2>pc.err </dev/null)
	rc=$?
}
cat >pc.s <<'PCEOF' || pc_unresolved "cannot write pc.s"
set a b c
echo "$# $1 $2 $3"
set x
echo "$# $1 ${2+S} ${3+S}"
set "a b" '' "c d"
echo "$#|$1|$2|$3"
set -- p q r s t u v w x y z A B
echo "$# ${10} ${13}"
pcx='1 2 3'
set $pcx
echo "$# $3"
PCEOF
pc_test
pcrun
want="3 a b c${nl}1 x  ${nl}3|a b||c d${nl}13 y B${nl}3 3"
[ "$out" = "$want" ] || pc_fail "got '$out'"
[ "$rc" -eq 0 ] || pc_fail "status $rc"
pc_pass
