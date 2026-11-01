# A nonexistent operand gets a diagnostic and a non-zero status, and the other operands are still listed.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.036
# MUTANT    ls.err_no_diag
# MUTANT    ls.err_status0
# MUTANT    ls.err_stdout_too
# MUTANT    ls.err_stops
# MUTANT    ls.err_stdout_only
# VARIANT   ls.err_status2
# VARIANT   ls.err_verbose
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.036
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

LC_ALL=POSIX; export LC_ALL
mkdir d || pc_fail "cannot create d"
: >d/seenfile || pc_fail "cannot create d/seenfile"
pc_test
ls nonexistent d >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "ls exited 0 although an operand does not exist"
[ -s err ] || pc_fail "no diagnostic"
slurp out
case $data in
*seenfile*) ;;
*) pc_fail "the existing operand was not listed" ;;
esac
# the diagnostic goes to standard error and nothing else to standard output
# than the listing of d (with or without its header)
while IFS= read -r l; do
	case $l in
	seenfile|'d:'|'') ;;
	'.'|'..') ;;
	*) pc_fail "unexpected line on standard output: $l" ;;
	esac
done <out
pc_pass
