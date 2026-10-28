# fc writes nothing to standard error when it lists or re-executes commands successfully.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.fc.021
. "$PC_CORE/pc.sh"
pc_begin XCU.fc.021
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
runi h1 'echo hi >/dev/null' 'fc -l 2>fcerr1 >/dev/null' 'fc -s -- -1 2>fcerr2 >/dev/null' 'exit 0'
[ -f fcerr1 ] && [ -f fcerr2 ] || pc_fail "the commands were not run"
[ ! -s fcerr1 ] || pc_fail "fc -l wrote to standard error: $(cat fcerr1)"
[ ! -s fcerr2 ] || pc_fail "fc -s wrote to standard error: $(cat fcerr2)"
pc_pass
