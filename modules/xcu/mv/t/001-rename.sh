# mv source_file target_file renames a file: the target has the contents, the source name is gone, nothing is written to standard output.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mv.001
. "$PC_CORE/pc.sh"
pc_begin XCU.mv.001
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
pc_test
mv a b >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "mv exited with status $rc"
[ ! -e a ] || pc_fail "source still exists"
slurp b
[ "$data" = "x${nl}" ] || pc_fail "target missing or wrong"
[ ! -s out ] || pc_fail "mv wrote to standard output"
pc_pass
