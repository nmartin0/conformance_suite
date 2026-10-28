# A suffix that does not match, that equals the whole remaining string, that is empty, or that is longer than the string leaves the string alone.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.basename.005
. "$PC_CORE/pc.sh"
pc_begin XCU.basename.005
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

chk() {
	if [ "$#" -eq 3 ]; then
		basename "$1" "$2" >o 2>e </dev/null
		rc=$?
		want=$3
		show="'$1' '$2'"
	else
		basename "$1" >o 2>e </dev/null
		rc=$?
		want=$2
		show="'$1'"
	fi
	[ "$rc" -eq 0 ] || pc_fail "basename $show exited with status $rc"
	slurp o
	[ "$data" = "$want$nl" ] || pc_fail "basename $show wrote '$data', expected '$want'"
}

pc_test
chk abc x abc
chk a.c a.c a.c
chk abc "" abc
chk a.c .cc a.c
chk /p/a.c a.c a.c
chk abc b abc
pc_pass
