# cat writes the contents of its file operands to standard output in operand order, byte for byte, and nothing else.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cat.001
. "$PC_CORE/pc.sh"
pc_begin XCU.cat.001
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

printf 'A1\nA2\n' >a || pc_fail "cannot create a"
printf 'B' >b || pc_fail "cannot create b"
printf '\001\377x\n' >c || pc_fail "cannot create c"
printf 'A1\nA2\nB\001\377x\n' >exp || pc_fail "cannot create exp"
pc_test
cat a b c >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cat exited with status $rc"
slurp exp
want=$data
slurp out
[ "$data" = "$want" ] || pc_fail "output is not the files joined in order"
pc_pass
