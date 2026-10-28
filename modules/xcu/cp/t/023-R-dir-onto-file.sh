# With -R, copying a directory onto an existing non-directory is an error that leaves the file alone.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cp.023
. "$PC_CORE/pc.sh"
pc_begin XCU.cp.023
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

printf 'orig\n' >f || pc_fail "cannot create f"
mkdir d || pc_fail "cannot create d"
printf 'x\n' >d/g || pc_fail "cannot create d/g"
pc_test
cp -R d f >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "cp -R exited 0 for a directory onto a regular file"
[ -f f ] || pc_fail "f is no longer a regular file"
slurp f
[ "$data" = "orig${nl}" ] || pc_fail "f was modified"
pc_pass
