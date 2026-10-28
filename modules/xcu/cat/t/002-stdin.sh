# With no file operands cat copies standard input to standard output.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cat.002
. "$PC_CORE/pc.sh"
pc_begin XCU.cat.002
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

pc_test
printf 'x\ny' | cat >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cat exited with status $rc"
slurp out
[ "$data" = "x${nl}y<noeol>" ] || pc_fail "standard input was not copied exactly"
pc_pass
