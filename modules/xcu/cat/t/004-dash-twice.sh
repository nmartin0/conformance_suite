# cat accepts several - operands and does not reopen standard input: with a regular file as input the second - adds nothing.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cat.004
. "$PC_CORE/pc.sh"
pc_begin XCU.cat.004
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

printf 'A\n' >a || pc_fail "cannot create a"
printf 'B\n' >b || pc_fail "cannot create b"
printf 'C\n' >c || pc_fail "cannot create c"
printf 'M1\nM2\n' >in || pc_fail "cannot create in"
pc_test
cat a - b - c <in >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cat exited with status $rc"
slurp out
[ "$data" = "A${nl}M1${nl}M2${nl}B${nl}C${nl}" ] || pc_fail "standard input was reread or the second - misbehaved"
pc_pass
