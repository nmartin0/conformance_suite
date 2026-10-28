# cat reads files of any type, so a character special file such as the null device contributes no bytes.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cat.007
. "$PC_CORE/pc.sh"
pc_begin XCU.cat.007
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
pc_test
cat a /dev/null b >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cat exited with status $rc"
slurp out
[ "$data" = "A${nl}B${nl}" ] || pc_fail "output differs"
pc_pass
