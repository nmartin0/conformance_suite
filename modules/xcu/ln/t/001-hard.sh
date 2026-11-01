# ln source_file target_file makes a second name for the same file: same file, link count 2, no output.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ln.001
# MUTANT    ln.hard_copy
# MUTANT    ln.hard_symbolic
# MUTANT    ln.hard_noop
# MUTANT    ln.ok_status1
# MUTANT    ln.ok_stdout
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
[ -e b ] || pc_fail "b was not created"
[ a -ef b ] || pc_fail "a and b are not the same file"
[ ! -h b ] || pc_fail "b is a symbolic link"
set -- $(ls -ld a)
[ "$2" = 2 ] || pc_fail "link count is $2, expected 2"
slurp b
[ "$data" = "data${nl}" ] || pc_fail "b has wrong contents"
[ ! -s out ] || pc_fail "ln wrote to standard output"
pc_pass
