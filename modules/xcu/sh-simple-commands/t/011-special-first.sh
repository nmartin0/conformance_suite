# A special built-in is found before any file in PATH: an executable named
# exit in a PATH directory is not run for exit 3.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-simple-commands.011
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-simple-commands.011
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
printf '%s\n' ': >pc.mark.exit' >pc.bin/exit || pc_unresolved "write"
chmod 755 pc.bin/exit || pc_unresolved "chmod"
pcscript "PATH=$PWD/pc.bin:\$PATH" 'exit 3' 
pc_test
pcrun
[ "$rc" -eq 3 ] || pc_fail "status $rc, expected 3"
[ -e pc.mark.exit ] && pc_fail "the file in PATH was run instead of the special built-in"
pc_pass
