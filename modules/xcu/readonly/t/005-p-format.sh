# readonly -p writes readonly name=value for each read-only variable that is
# set (value quoted for reading back) and readonly name for one that is
# unset; reading it back into a new shell makes the variable read-only again
# with its value.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.readonly.005
. "$PC_CORE/pc.sh"
pc_begin XCU.readonly.005
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
pcscript 'readonly PCR1="a b"' 'readonly PCR2' 'readonly -p >pc.rp' 
pc_test
pcrun
[ "$rc" -eq 0 ] || pc_fail "setup shell failed with $rc"
grep '^readonly PCR1=' pc.rp >pc.l1 || pc_fail "no 'readonly PCR1=' line"
grep '^readonly PCR2$' pc.rp >/dev/null || pc_fail "no 'readonly PCR2' line for the unset read-only name"
pcscript '. ./pc.l1' 'echo "[$PCR1]"' '(PCR1=z) 2>/dev/null' 'echo "rc=$? v=$PCR1"'
pcrun
case $out in
"[a b]${nl}rc="[1-9]*" v=a b") ;;
*) pc_fail "reading the line back gave '$out'" ;;
esac
pc_pass
