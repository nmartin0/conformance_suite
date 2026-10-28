# The directory of a file in the root directory is the root, and of the root itself is the root.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.dirname.004
. "$PC_CORE/pc.sh"
pc_begin XCU.dirname.004
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
	dirname "$1" >o 2>e </dev/null
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "dirname '$1' exited with status $rc"
	slurp o
	[ "$data" = "$2$nl" ] || pc_fail "dirname '$1' wrote '$data', expected '$2'"
}

pc_test
chk / /
chk /a /
chk /a/ /
pc_pass
