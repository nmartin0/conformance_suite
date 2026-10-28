# Trailing slashes that are not also leading slashes are not counted.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.dirname.003
. "$PC_CORE/pc.sh"
pc_begin XCU.dirname.003
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
chk /usr/lib/ /usr
chk usr/lib// usr
chk usr/ .
chk /usr/ /
pc_pass
