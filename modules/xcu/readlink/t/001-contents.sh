# readlink writes the contents of a symbolic link, unresolved, followed by a newline.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.readlink.001
. "$PC_CORE/pc.sh"
pc_begin XCU.readlink.001
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

ln -s ../a/./b lk || pc_fail "cannot create symbolic link"
pc_test
readlink lk >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "readlink exited with status $rc"
slurp out
[ "$data" = "../a/./b${nl}" ] || pc_fail "output is wrong"
pc_pass
