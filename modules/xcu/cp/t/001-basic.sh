# cp source_file target_file creates the target with the source contents, leaves the source unchanged, and writes nothing to standard output.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cp.001
. "$PC_CORE/pc.sh"
pc_begin XCU.cp.001
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

printf 'hello\nworld' >src || pc_fail "cannot create src"
pc_test
cp src dst >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cp exited with status $rc"
slurp dst
[ "$data" = "hello${nl}world<noeol>" ] || pc_fail "copy has wrong contents"
slurp src
[ "$data" = "hello${nl}world<noeol>" ] || pc_fail "source was changed"
[ ! -s out ] || pc_fail "cp wrote to standard output"
pc_pass
