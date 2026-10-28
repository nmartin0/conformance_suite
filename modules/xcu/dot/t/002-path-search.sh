# A file operand without a slash is looked for in the directories of PATH
# and need not be executable; the current directory is not searched unless
# PATH names it.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.dot.002
. "$PC_CORE/pc.sh"
pc_begin XCU.dot.002
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
mkdir pc.bin || pc_unresolved "mkdir"
printf '%s\n' 'pcw=viapath' >pc.bin/pc.d2 || pc_unresolved "cannot write pc.bin/pc.d2"
chmod 644 pc.bin/pc.d2 || pc_unresolved "chmod"
printf '%s\n' 'pcw=incwd' >pc.d2 || pc_unresolved "cannot write pc.d2"
pcscript 'oldp=$PATH' "PATH=$PWD/pc.bin" '. pc.d2' 'rc=$?' 'PATH=$oldp' 'echo "$rc $pcw"' 
pc_test
pcrun
[ "$out" = "0 viapath" ] || pc_fail "got '$out', expected '0 viapath' (file in the PATH directory, not the one in the working directory)"
pc_pass
