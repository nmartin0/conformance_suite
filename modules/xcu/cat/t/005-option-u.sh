# cat accepts -u and still writes the same bytes.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cat.005
. "$PC_CORE/pc.sh"
pc_begin XCU.cat.005
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

printf 'A1\nA2\n' >a || pc_fail "cannot create a"
printf 'B' >b || pc_fail "cannot create b"
pc_test
cat -u a b >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cat -u exited with status $rc"
slurp out
[ "$data" = "A1${nl}A2${nl}B<noeol>" ] || pc_fail "cat -u output differs"
printf 'q\nr\n' | cat -u >out2 2>err2
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cat -u (standard input) exited with status $rc"
slurp out2
[ "$data" = "q${nl}r${nl}" ] || pc_fail "cat -u did not copy standard input"
pc_pass
