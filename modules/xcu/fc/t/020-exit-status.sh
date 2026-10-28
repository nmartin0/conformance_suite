# fc returns zero after a successful listing, a nonzero status for an error, and otherwise the status of the commands it executed.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.fc.020
. "$PC_CORE/pc.sh"
pc_begin XCU.fc.020
# Run an interactive shell on a script of the commands given, one per
# line, with a history file of its own; its standard output is left in out.
runi() {
	hist=$1
	shift
	printf '%s\n' "$@" >script
	HISTFILE=$PWD/$hist ENV= "${PC_SH:-sh}" -i <script >out 2>err
}
tab=$(printf '\t')
pc_test
runi h1 '(exit 7)' 'fc -s -- -1' 'echo "st=$?"' 'fc -l >/dev/null' 'echo "ls=$?"' 'fc -l pcnosuchcmdstring >/dev/null 2>&1' 'echo "bad=$?"' 'exit 0'
grep -x -F 'st=7' out >/dev/null || pc_fail "the status of the re-executed command was not returned: $(cat out)"
grep -x -F 'ls=0' out >/dev/null || pc_fail "the listing did not return zero: $(cat out)"
grep -x -E 'bad=[1-9][0-9]*' out >/dev/null || pc_fail "the error did not return a nonzero status: $(cat out)"
pc_pass
