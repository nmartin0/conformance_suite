# A continue whose operand is not a positive decimal integer returns a non-
# zero status (or the shell exits with a non-zero status).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.continue.005
. "$PC_CORE/pc.sh"
pc_begin XCU.continue.005
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
printf '%s\n' 'echo ran' >pc.probe || pc_unresolved "cannot write pc.probe"
pcprobe=$(${PC_SH:-sh} pc.probe 2>/dev/null </dev/null)
[ "$pcprobe" = ran ] || pc_unresolved "cannot run the shell under test"
pc_test
for arg in 0 abc; do
	pcscript 'for i in 1 2; do' "  continue $arg" '  echo "rc=$?"' 'done' 'echo after'
	pcrun
	if [ "$rc" -eq 0 ]; then
		first=${out%%"$nl"*}
		case $first in
		# a shell that leaves the loop after the error shows only 'after':
		# the status of the utility cannot be seen then
		rc=[1-9]*|after) ;;
		*) pc_fail "continue $arg: neither the shell failed nor continue returned non-zero (out='$out')" ;;
		esac
	fi
done
pc_pass
