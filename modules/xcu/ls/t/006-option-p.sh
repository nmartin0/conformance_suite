# ls -p writes a slash after each directory name.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.006
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.006
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
mkdir d d/sub || pc_fail "cannot create directories"
: >d/f || pc_fail "cannot create d/f"
pc_test
ls -p d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls -p exited with status $rc"
slurp out
[ "$data" = "f${nl}sub/${nl}" ] || pc_fail "ls -p output is wrong"
pc_pass
