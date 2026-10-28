# readlink -n omits the trailing newline.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.readlink.002
. "$PC_CORE/pc.sh"
pc_begin XCU.readlink.002
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
readlink -n lk >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "readlink -n exited with status $rc"
slurp out
[ "$data" = "../a/./b<noeol>" ] || pc_fail "output is wrong"
pc_pass
