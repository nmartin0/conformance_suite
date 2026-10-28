# cp onto an existing regular file replaces its contents completely, with no remnant of longer old data.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cp.002
. "$PC_CORE/pc.sh"
pc_begin XCU.cp.002
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

printf 'new\n' >src || pc_fail "cannot create src"
printf 'a much longer old content that must vanish\n' >dst || pc_fail "cannot create dst"
pc_test
cp src dst >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cp exited with status $rc"
slurp dst
[ "$data" = "new${nl}" ] || pc_fail "target was not truncated and rewritten"
pc_pass
