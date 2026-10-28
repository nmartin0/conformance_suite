# The words name=word given to readonly are expanded as assignments: tilde
# after = and colons, no field splitting.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.readonly.007
. "$PC_CORE/pc.sh"
pc_begin XCU.readonly.007
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
pcscript 'HOME=/pc/h' 'pcx="p q"' 'readonly PCRA=$pcx PCRB=~/x:~/y' 'echo "[$PCRA][$PCRB]"' 
pc_test
pcrun
[ "$out" = "[p q][/pc/h/x:/pc/h/y]" ] || pc_fail "got '$out'"
pc_pass
