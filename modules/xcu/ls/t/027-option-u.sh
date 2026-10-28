# With -t, -u sorts by the time of last access instead of modification.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.027
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.027
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
touch -m -t 200101010101 d/p || pc_fail "cannot set time"
touch -m -t 200201010101 d/q || pc_fail "cannot set time"
touch -m -t 200301010101 d/r || pc_fail "cannot set time"
touch -a -t 200501010101 d/p || pc_fail "cannot set time"
touch -a -t 200401010101 d/q || pc_fail "cannot set time"
touch -a -t 200301010101 d/r || pc_fail "cannot set time"
pc_test
ls -t d >out 2>err </dev/null
slurp out
[ "$data" = "r${nl}q${nl}p${nl}" ] || pc_fail "ls -t (modification time) output is wrong"
ls -tu d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls -tu exited with status $rc"
slurp out
[ "$data" = "p${nl}q${nl}r${nl}" ] || pc_fail "ls -tu output is wrong"
pc_pass
