# A nonexistent operand gets a diagnostic and a non-zero status, and the other operands are still listed.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.036
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.036
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

LC_ALL=POSIX; export LC_ALL
mkdir d || pc_fail "cannot create d"
: >d/seenfile || pc_fail "cannot create d/seenfile"
pc_test
ls nonexistent d >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "ls exited 0 although an operand does not exist"
[ -s err ] || pc_fail "no diagnostic"
slurp out
case $data in
*seenfile*) ;;
*) pc_fail "the existing operand was not listed" ;;
esac
pc_pass
