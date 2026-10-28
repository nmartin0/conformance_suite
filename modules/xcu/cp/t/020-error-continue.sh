# If one source cannot be copied, cp diagnoses it, copies the remaining sources, and exits non-zero.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cp.020
. "$PC_CORE/pc.sh"
pc_begin XCU.cp.020
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

printf 'A\n' >a || pc_fail "cannot create a"
printf 'B\n' >b || pc_fail "cannot create b"
mkdir d || pc_fail "cannot create d"
pc_test
cp a nonexistent b d >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "cp exited 0 although a source does not exist"
[ -s err ] || pc_fail "no diagnostic"
slurp d/a
[ "$data" = "A${nl}" ] || pc_fail "d/a not copied"
slurp d/b
[ "$data" = "B${nl}" ] || pc_fail "d/b not copied"
pc_pass
