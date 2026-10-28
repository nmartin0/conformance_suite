# A command name with a slash is run as given with no PATH search, and a
# name without a slash is searched in PATH, not in the working directory.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-simple-commands.014
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-simple-commands.014
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
printf '%s\n' ': >"pc.mark.$1"' >pc.bin/pcutil2 || pc_unresolved "write"
chmod 755 pc.bin/pcutil2 || pc_unresolved "chmod"
pcscript 'oldp=$PATH' "PATH=$PWD/pc.bin:\$PATH" 'pcutil2 a' 'PATH=/nonexistent.pc' 'pc.bin/pcutil2 b' 'PATH=$oldp' './pcutil2 c' 'echo "rc=$?"' 
pc_test
pcrun
[ -e pc.mark.a ] || pc_fail "the name without a slash was not found through PATH"
[ -e pc.mark.b ] || pc_fail "the name with a slash was not run with PATH unusable"
[ -e pc.mark.c ] && pc_fail "./pcutil2 ran a file from PATH"
pc_pass
