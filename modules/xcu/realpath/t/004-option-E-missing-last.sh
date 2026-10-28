# realpath -E accepts a missing last component when the directory part exists, expanding links in that part.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.realpath.004
. "$PC_CORE/pc.sh"
pc_begin XCU.realpath.004
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

root=$(pwd -P) || pc_fail "cannot determine the directory"
mkdir real real/sub || pc_fail "cannot create directories"
: >real/f || pc_fail "cannot create real/f"
ln -s f real/fl || pc_fail "cannot create real/fl"
ln -s real/sub lnk2 || pc_fail "cannot create lnk2"
pc_test
realpath -E real/new >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "realpath -E real/new exited with status $rc"
slurp out
[ "$data" = "$root/real/new${nl}" ] || pc_fail "real/new gave: $data"
realpath -E lnk2/new >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "realpath -E lnk2/new exited with status $rc"
slurp out
[ "$data" = "$root/real/sub/new${nl}" ] || pc_fail "lnk2/new gave: $data"
realpath -E real/f >out 2>err </dev/null
slurp out
[ "$data" = "$root/real/f${nl}" ] || pc_fail "an existing file with -E gave: $data"
pc_pass
