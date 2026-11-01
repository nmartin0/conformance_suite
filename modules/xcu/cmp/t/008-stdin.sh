# A file operand of - makes cmp read standard input, as either operand.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cmp.008
# MUTANT    cmp.stdin_literal
# MUTANT    cmp.stdin_file1_only
# MUTANT    cmp.stdin_file2_only
# MUTANT    cmp.stdin_empty
# MUTANT    cmp.stdin_name_dup
# MUTANT    cmp.stdin_consumed
# MUTANT    cmp.stdin_as_file2
# VARIANT   cmp.stdin_name
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
printf 'abd\n' >g || pc_fail "cannot create g"
pc_test
printf 'abc\n' | cmp - f >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cmp - f exited with status $rc for identical data"
printf 'abc\n' | cmp f - >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cmp f - exited with status $rc for identical data"
[ ! -s out ] || pc_fail "output written for identical data"
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
printf 'abd\n' | cmp - f >out 2>err
rc=$?
[ "$rc" -eq 1 ] || pc_fail "cmp - f exited with status $rc for different data"
slurp out
case $data in
"- f differ: char 3, line 1${nl}" | "stdin f differ: char 3, line 1${nl}") ;;
*) pc_fail "output with - first is: $data" ;;
esac
# the data of standard input is file1 when - comes first, file2 otherwise
printf 'abd\n' | cmp -l - f >out 2>err
rc=$?
[ "$rc" -eq 1 ] || pc_fail "cmp -l - f exited with status $rc for different data"
slurp out
case $data in
"3 144 143${nl}") ;;
*) pc_fail "cmp -l - f output is: $data" ;;
esac
printf 'abd\n' | cmp -l f - >out 2>err
slurp out
case $data in
"3 143 144${nl}") ;;
*) pc_fail "cmp -l f - output is: $data" ;;
esac
# standard input is used only when an operand is -: here it is left alone
rest=$(printf 'xyz\n' | { cmp f g >/dev/null 2>&1; cat; })
[ "$rest" = xyz ] || pc_fail "cmp read standard input without an operand of -"
pc_pass
