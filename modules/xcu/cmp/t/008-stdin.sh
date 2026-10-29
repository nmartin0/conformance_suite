# A file operand of - makes cmp read standard input, as either operand.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cmp.008
. "$PC_CORE/pc.sh"
pc_begin XCU.cmp.008
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

printf 'abc\n' >f || pc_fail "cannot create f"
pc_test
printf 'abc\n' | cmp - f >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cmp - f exited with status $rc for identical data"
printf 'abd\n' | cmp f - >out 2>err
rc=$?
[ "$rc" -eq 1 ] || pc_fail "cmp f - exited with status $rc for different data"
slurp out
# the name written for the operand - is not given (Q-206): the operand itself
# and a word for standard input are both accepted
case $data in
"f - differ: char 3, line 1${nl}" | "f stdin differ: char 3, line 1${nl}") ;;
*) pc_fail "output is: $data" ;;
esac
pc_pass
