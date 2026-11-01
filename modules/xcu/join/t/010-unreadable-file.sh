# A file operand that cannot be read gives a diagnostic on standard error, nothing on standard output, and a nonzero status.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.join.010
# MUTANT    join.missing1_status0
# MUTANT    join.missing2_status0
# MUTANT    join.missing_no_diag
# MUTANT    join.missing_diag_stdout
# MUTANT    join.missing_a_ok
# VARIANT   join.status2
# VARIANT   join.status255
# VARIANT   join.missing_partial
#
# With -a the lines of the readable file may or may not be written before
# the failure, so only the status and the diagnostic are judged there.
. "$PC_CORE/pc.sh"
pc_begin XCU.join.010
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a 1\n' >f1 || pc_fail "cannot create input"
[ ! -e nosuchfile ] || pc_unresolved "nosuchfile exists"
pc_test
join f1 nosuchfile >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "status 0 although file2 could not be read"
[ -f out ] && [ ! -s out ] || pc_fail "standard output was not empty although file2 could not be read"
[ -s err ] || pc_fail "no diagnostic on standard error although file2 could not be read"
join nosuchfile f1 >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "status 0 although file1 could not be read"
[ -f out ] && [ ! -s out ] || pc_fail "standard output was not empty although file1 could not be read"
[ -s err ] || pc_fail "no diagnostic on standard error although file1 could not be read"
join -a 1 f1 nosuchfile >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "status 0 with -a 1 although file2 could not be read"
[ -s err ] || pc_fail "no diagnostic with -a 1 although file2 could not be read"
join -v 2 nosuchfile f1 >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "status 0 with -v 2 although file1 could not be read"
[ -s err ] || pc_fail "no diagnostic with -v 2 although file1 could not be read"
pc_pass
