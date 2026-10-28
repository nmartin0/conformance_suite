# A file operand that contains a slash is used as given, with no PATH
# search.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.dot.003
. "$PC_CORE/pc.sh"
pc_begin XCU.dot.003
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
mkdir pc.sub || pc_unresolved "mkdir"
printf '%s\n' 'pcw=sub' >pc.sub/pc.d3 || pc_unresolved "write"
printf '%s\n' 'pcw=cwd' >pc.d3 || pc_unresolved "write"
pcscript 'oldp=$PATH' 'PATH=/nonexistent.pc' '. ./pc.d3' 'a=$pcw' '. pc.sub/pc.d3' 'PATH=$oldp' 'echo "$a $pcw"' 
pc_test
pcrun
[ "$out" = "cwd sub" ] || pc_fail "got '$out', expected 'cwd sub'"
pc_pass
