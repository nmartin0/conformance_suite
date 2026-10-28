# A symbolic link to a directory given as operand is followed (its contents listed) when none of -d, -F, -l is used.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.030
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.030
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
: >d/sub/x || pc_fail "cannot create d/sub/x"
ln -s sub d/sl || pc_fail "cannot create symbolic link"
pc_test
ls d/sl >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls exited with status $rc"
slurp out
[ "$data" = "x${nl}" ] || pc_fail "the contents of the directory were not listed"
pc_pass
