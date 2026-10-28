# An intrinsic utility such as umask is not looked up in PATH: an executable
# named umask in a PATH directory is not run.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-simple-commands.013
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-simple-commands.013
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
printf '%s\n' ': >pc.mark.umask' >pc.bin/umask || pc_unresolved "write"
chmod 755 pc.bin/umask || pc_unresolved "chmod"
pcscript "PATH=$PWD/pc.bin:\$PATH" 'umask >/dev/null' 'echo done' 
pc_test
pcrun
[ "$out" = done ] || pc_fail "got '$out'"
[ -e pc.mark.umask ] && pc_fail "the file in PATH was run instead of the intrinsic utility"
pc_pass
