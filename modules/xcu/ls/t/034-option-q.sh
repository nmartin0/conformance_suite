# ls -q writes a question mark for each nonprintable character of a file name, including a newline.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.034
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.034
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
n1=$(printf 'a\001b')
n2=$(printf 'c\nd')
: >"d/$n1" || pc_fail "cannot create the first file"
: >"d/$n2" || pc_fail "cannot create the second file"
pc_test
ls -q d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls -q exited with status $rc"
slurp out
[ "$data" = "a?b${nl}c?d${nl}" ] || pc_fail "ls -q output is wrong"
pc_pass
