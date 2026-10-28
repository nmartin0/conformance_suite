# For files that differ cmp writes the names, the byte number and the line number of the first difference, both counted from 1, and exits 1.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cmp.002
. "$PC_CORE/pc.sh"
pc_begin XCU.cmp.002
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

printf 'abc\ndef\n' >f1 || pc_fail "cannot create f1"
printf 'abc\ndXf\n' >f2 || pc_fail "cannot create f2"
printf 'x\n' >g1 || pc_fail "cannot create g1"
printf 'y\n' >g2 || pc_fail "cannot create g2"
printf 'a\nb\nc\nX\n' >h1 || pc_fail "cannot create h1"
printf 'a\nb\nc\nY\n' >h2 || pc_fail "cannot create h2"
pc_test
cmp f1 f2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "cmp exited with status $rc, expected 1"
slurp out
[ "$data" = "f1 f2 differ: char 6, line 2${nl}" ] || pc_fail "output is: $data"
cmp g1 g2 >out 2>err </dev/null
slurp out
[ "$data" = "g1 g2 differ: char 1, line 1${nl}" ] || pc_fail "output for the first byte is: $data"
cmp h1 h2 >out 2>err </dev/null
slurp out
[ "$data" = "h1 h2 differ: char 7, line 4${nl}" ] || pc_fail "output for the last line is: $data"
pc_pass
