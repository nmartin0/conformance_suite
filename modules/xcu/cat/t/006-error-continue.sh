# When one operand cannot be read, cat reports it on standard error, still writes the other operands, and exits non-zero.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cat.006
. "$PC_CORE/pc.sh"
pc_begin XCU.cat.006
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
cat a nonexistent b >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "cat exited 0 although an operand does not exist"
[ -s err ] || pc_fail "no diagnostic on standard error"
slurp out
[ "$data" = "A${nl}B${nl}" ] || pc_fail "the readable operands were not written"
pc_pass
