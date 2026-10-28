# exit with no operand uses the exit status of the last command executed.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.exit.002
. "$PC_CORE/pc.sh"
pc_begin XCU.exit.002
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
pc_test
for pair in "false:1" "(exit 9):9" "true:0" "(exit 77):77"; do
	cmd=${pair%:*}
	want=${pair##*:}
	pcscript "$cmd" 'exit'
	pcrun
	[ "$rc" -eq "$want" ] || pc_fail "'$cmd' then exit gave $rc, expected $want"
done
pc_pass
