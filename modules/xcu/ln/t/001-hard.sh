# ln source_file target_file makes a second name for the same file: same file, link count 2, no output.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ln.001
. "$PC_CORE/pc.sh"
pc_begin XCU.ln.001
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

printf 'data\n' >a || pc_fail "cannot create a"
pc_test
ln a b >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ln exited with status $rc"
[ a -ef b ] || pc_fail "a and b are not the same file"
set -- $(ls -ld a)
[ "$2" = 2 ] || pc_fail "link count is $2, expected 2"
slurp b
[ "$data" = "data${nl}" ] || pc_fail "b has wrong contents"
[ ! -s out ] || pc_fail "ln wrote to standard output"
pc_pass
