# With no operand ls lists the current directory; a file operand is written as given, and a dot file is listed when named.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.037
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.037
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
: >d/a || pc_fail "cannot create d/a"
: >d/.h || pc_fail "cannot create d/.h"
pc_test
(cd d && ls) >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls exited with status $rc"
slurp out
[ "$data" = "a${nl}" ] || pc_fail "ls without operand wrote something else"
ls ./d/a d/.h >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls with file operands exited with status $rc"
slurp out
[ "$data" = "./d/a${nl}d/.h${nl}" ] || pc_fail "file operands were not written as given"
pc_pass
