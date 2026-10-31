# A file operand that cannot be read gives a nonzero status, and the other operands are still counted.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.wc.009
# MUTANT    wc.miss_status0
# MUTANT    wc.miss_abort
# MUTANT    wc.miss_no_diag
# MUTANT    wc.miss_diag_stdout
# MUTANT    wc.miss_discard
# MUTANT    wc.miss_status_last
# MUTANT    wc.miss_status_first
# VARIANT   wc.padded
# VARIANT   wc.status2
# VARIANT   wc.status255
# VARIANT   wc.total_none_on_error
. "$PC_CORE/pc.sh"
pc_begin XCU.wc.009
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a b\n' >g || pc_fail "cannot create input"
[ ! -e nosuchfile ] || pc_unresolved "nosuchfile exists"
pc_test
# pc_try ARGS: run wc -l on the operands; status nonzero, a diagnostic on standard
# error, and standard output made only of entries (a count first), one of them for g
pc_try() {
	wc -l "$@" >out 2>err </dev/null
	rc=$?
	[ "$rc" -ne 0 ] || pc_fail "status 0 although a file could not be read (wc -l $*)"
	[ -s err ] || pc_fail "no diagnostic on standard error (wc -l $*)"
	found=
	while IFS= read -r line; do
		set -- $line
		case $1 in
		'' | *[!0-9]*) pc_fail "standard output holds a line that is not an entry: $line" ;;
		esac
		# the entry for g is a line of the count and the name (blanks around them are not fixed)
		if [ "$#" -eq 2 ] && [ "$1" = 1 ] && [ "$2" = g ]; then found=$((found + 1)); fi
	done <out
	[ -n "$found" ] || pc_fail "the readable file was not counted"
}
pc_try nosuchfile g
pc_try g nosuchfile
pc_try g nosuchfile g
[ "$found" -eq 2 ] || pc_fail "the readable file operand given twice was not counted twice"
pc_pass
