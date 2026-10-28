# unlink removes the named directory entry and leaves other names of the same file intact.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.unlink.001
. "$PC_CORE/pc.sh"
pc_begin XCU.unlink.001
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

printf 'x\n' >a || pc_fail "cannot create a"
ln a b || pc_fail "cannot create hard link"
printf 'y\n' >c || pc_fail "cannot create c"
pc_test
unlink a >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "unlink exited with status $rc"
[ ! -e a ] || pc_fail "a still exists"
slurp b
[ "$data" = "x${nl}" ] || pc_fail "b lost its contents"
unlink c >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "unlink c exited with status $rc"
[ ! -e c ] || pc_fail "c still exists"
[ ! -s out ] || pc_fail "unlink wrote to standard output"
pc_pass
