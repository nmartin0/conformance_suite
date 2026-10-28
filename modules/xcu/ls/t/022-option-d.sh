# ls -d writes a directory operand itself instead of its contents; with no operand it writes dot.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.022
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.022
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
: >d/f || pc_fail "cannot create d/f"
pc_test
ls -d d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls -d exited with status $rc"
slurp out
[ "$data" = "d${nl}" ] || pc_fail "ls -d d wrote something else"
(cd d && ls -d) >out 2>err </dev/null
slurp out
[ "$data" = ".${nl}" ] || pc_fail "ls -d without operand wrote something else"
pc_pass
