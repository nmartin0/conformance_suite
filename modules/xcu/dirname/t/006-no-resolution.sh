# dirname does not look at the file system: symbolic links are not resolved and the file need not exist.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.dirname.006
. "$PC_CORE/pc.sh"
pc_begin XCU.dirname.006
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

mkdir real || pc_fail "cannot create real"
ln -s real lnk || pc_fail "cannot create symbolic link"
pc_test
chk lnk/x lnk
chk lnk .
pc_pass
