# realpath writes the absolute pathname with symbolic links followed and dot and dot-dot resolved, as the realpath() function would, followed by a newline.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.realpath.001
. "$PC_CORE/pc.sh"
pc_begin XCU.realpath.001
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
realpath lnk2/../f >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "realpath exited with status $rc"
slurp out
[ "$data" = "$root/real/f${nl}" ] || pc_fail "lnk2/../f gave: $data"
realpath ./real/fl >out 2>err </dev/null
slurp out
[ "$data" = "$root/real/f${nl}" ] || pc_fail "./real/fl gave: $data"
[ ! -s err ] || pc_fail "diagnostic written"
pc_pass
