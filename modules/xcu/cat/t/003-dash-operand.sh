# A file operand of - makes cat read standard input at that point in the sequence.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cat.003
. "$PC_CORE/pc.sh"
pc_begin XCU.cat.003
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
pc_test
printf 'M\n' | cat a - b >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cat exited with status $rc"
slurp out
[ "$data" = "A${nl}M${nl}B${nl}" ] || pc_fail "standard input was not read between the two files"
pc_pass
