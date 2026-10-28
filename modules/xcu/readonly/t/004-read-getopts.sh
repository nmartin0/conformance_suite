# The read and getopts utilities cannot change a read-only variable: each
# returns a value greater than one, the shell does not exit, and the value
# stays.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.readonly.004
. "$PC_CORE/pc.sh"
pc_begin XCU.readonly.004
nl='
'
# pcscript: write the arguments, one per line, to pc.s
pcscript() {
	printf '%s\n' "$@" >pc.s || pc_unresolved "cannot write pc.s"
}
# pcrun: run pc.s in a fresh shell; out = its standard output, rc = its
# exit status, its standard error is left in pc.err.
pcrun() {
	out=$(${PC_SH:-sh} pc.s 2>pc.err </dev/null)
	rc=$?
}
printf '%s\n' newvalue >pc.rin || pc_unresolved "write"
pcscript 'readonly PCR=1' 'read PCR <pc.rin' 'echo "a=$?"' 'echo "v1=$PCR"' 'set -- -a' 'getopts a PCR' 'echo "b=$?"' 'echo "v2=$PCR"' 
pc_test
pcrun
a=$(printf '%s\n' "$out" | sed -n 's/^a=//p')
b=$(printf '%s\n' "$out" | sed -n 's/^b=//p')
v1=$(printf '%s\n' "$out" | sed -n 's/^v1=//p')
v2=$(printf '%s\n' "$out" | sed -n 's/^v2=//p')
[ -n "$a" ] && [ -n "$b" ] || pc_fail "the shell did not get past the failing utilities (status $rc, out '$out')"
[ "$a" -gt 1 ] || pc_fail "read returned $a, expected more than 1"
[ "$b" -gt 1 ] || pc_fail "getopts returned $b, expected more than 1"
[ "$v1" = 1 ] && [ "$v2" = 1 ] || pc_fail "value changed (v1='$v1' v2='$v2')"
pc_pass
