# readlink does not follow the link: for a link to a link it writes only the first link text, and a dangling link is fine.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.readlink.003
. "$PC_CORE/pc.sh"
pc_begin XCU.readlink.003
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

: >f || pc_fail "cannot create f"
ln -s f l1 || pc_fail "cannot create l1"
ln -s l1 l2 || pc_fail "cannot create l2"
ln -s nonexistent dl || pc_fail "cannot create dl"
pc_test
readlink l2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "readlink l2 exited with status $rc"
slurp out
[ "$data" = "l1${nl}" ] || pc_fail "l2: output is wrong"
readlink dl >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "readlink dl exited with status $rc"
slurp out
[ "$data" = "nonexistent${nl}" ] || pc_fail "dl: output is wrong"
pc_pass
