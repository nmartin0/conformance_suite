# A file operand that cannot be read gives a diagnostic and a nonzero status, and nothing on standard output.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tail.006
# MUTANT    tail.err_status0
# MUTANT    tail.err_status0_c
# MUTANT    tail.err_status0_r
# MUTANT    tail.err_status0_plus
# MUTANT    tail.err_no_diag
# MUTANT    tail.err_no_diag_c
# MUTANT    tail.err_no_diag_r
# MUTANT    tail.err_diag_stdout
# MUTANT    tail.err_output
# VARIANT   tail.err_status2
. "$PC_CORE/pc.sh"
pc_begin XCU.tail.006
LC_ALL=POSIX
export LC_ALL
NL='
'
[ ! -e nosuchfile ] || pc_unresolved "nosuchfile exists"
chk() {
	desc=$1
	shift
	rm -f out err
	"$@" >out 2>err </dev/null
	rc=$?
	[ -f out ] && [ -f err ] || pc_unresolved "cannot create the files that capture the output"
	[ "$rc" -ne 0 ] || pc_fail "$desc: status 0 for a file that cannot be read"
	[ -s err ] || pc_fail "$desc: no diagnostic on standard error"
	[ ! -s out ] || pc_fail "$desc: something was written to standard output"
}
pc_test
chk "no options" tail nosuchfile
chk "-n 1" tail -n 1 nosuchfile
chk "-n +1" tail -n +1 nosuchfile
chk "-c 1" tail -c 1 nosuchfile
chk "-c +1" tail -c +1 nosuchfile
chk "-r" tail -r nosuchfile
chk "-r -n 1" tail -r -n 1 nosuchfile
pc_pass
