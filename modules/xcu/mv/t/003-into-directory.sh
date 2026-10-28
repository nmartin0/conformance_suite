# mv source_file.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mv.003
. "$PC_CORE/pc.sh"
pc_begin XCU.mv.003
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

printf 'one\n' >a || pc_fail "cannot create a"
printf 'two\n' >b || pc_fail "cannot create b"
mkdir d || pc_fail "cannot create d"
pc_test
mv a b d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "mv exited with status $rc"
[ ! -e a ] && [ ! -e b ] || pc_fail "sources still exist"
slurp d/a
[ "$data" = "one${nl}" ] || pc_fail "d/a missing or wrong"
slurp d/b
[ "$data" = "two${nl}" ] || pc_fail "d/b missing or wrong"
pc_pass
