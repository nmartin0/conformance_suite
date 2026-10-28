# ls -t sorts by modification time, newest first, ties by name; with -r the oldest comes first.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.026
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.026
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
mkdir d e || pc_fail "cannot create directories"
touch -t 200101010101 d/a e/a || pc_fail "cannot set time"
touch -t 200201010101 d/b d/x e/b || pc_fail "cannot set time"
touch -t 200301010101 d/c e/c || pc_fail "cannot set time"
pc_test
ls -t d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls -t exited with status $rc"
slurp out
[ "$data" = "c${nl}b${nl}x${nl}a${nl}" ] || pc_fail "ls -t output is wrong"
ls -rt e >out 2>err </dev/null
slurp out
[ "$data" = "a${nl}b${nl}c${nl}" ] || pc_fail "ls -rt output is wrong"
pc_pass
