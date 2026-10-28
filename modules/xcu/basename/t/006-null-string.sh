# For a null string the result is a dot or a null string, and the status is zero.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.basename.006
. "$PC_CORE/pc.sh"
pc_begin XCU.basename.006
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
basename "" >o 2>e </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "basename '' exited with status $rc"
slurp o
case $data in
"$nl"|".$nl") ;;
*) pc_fail "basename '' wrote '$data'" ;;
esac
basename "" .c >o 2>e </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "basename '' .c exited with status $rc"
slurp o
case $data in
"$nl"|".$nl") ;;
*) pc_fail "basename '' .c wrote '$data'" ;;
esac
pc_pass
