# mv onto an existing file replaces it, without prompting when standard input is not a terminal.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mv.002
. "$PC_CORE/pc.sh"
pc_begin XCU.mv.002
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

printf 'new\n' >a || pc_fail "cannot create a"
printf 'old\n' >b || pc_fail "cannot create b"
pc_test
mv a b >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "mv exited with status $rc"
[ ! -e a ] || pc_fail "source still exists"
slurp b
[ "$data" = "new${nl}" ] || pc_fail "target was not replaced"
pc_pass
