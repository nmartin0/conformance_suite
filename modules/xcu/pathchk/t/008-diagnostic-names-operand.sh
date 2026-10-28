# The diagnostic names the offending operand, and one bad operand makes the status non-zero even when the others are fine.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pathchk.008
. "$PC_CORE/pc.sh"
pc_begin XCU.pathchk.008
nl='
'
slurp() {
	data=
	l=
	while IFS= read -r l; do
		data=$data$l$nl
		l=
	done <"$1"
	[ -z "$l" ] || data=$data$l'<noeol>'
	return 0
}

pc_test
pathchk -p ok-name 'bad name' ok2 >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "pathchk exited 0 although one operand is bad"
slurp err
case $data in
*"bad name"*) ;;
*) pc_fail "the diagnostic does not name the operand: $data" ;;
esac
pathchk -p a b c >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "pathchk -p a b c exited with status $rc"
pc_pass
