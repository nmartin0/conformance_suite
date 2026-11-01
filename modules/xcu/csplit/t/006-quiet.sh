# csplit -s writes no size lines.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.csplit.006
# MUTANT    csplit.s_ignored
# MUTANT    csplit.s_to_stderr
# MUTANT    csplit.s_partial
# MUTANT    csplit.s_status1
. "$PC_CORE/pc.sh"
pc_begin XCU.csplit.006
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '%s\n' 1 2 3 4 5 6 7 8 9 10 >in10 || pc_fail "cannot create input"
pc_test
csplit -s in10 5 8 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "csplit -s gave status $rc"
[ -s out ] && pc_fail "-s still wrote to standard output"
[ -s err ] && pc_fail "-s with no error wrote to standard error"
[ -s xx00 ] && [ -s xx01 ] && [ -s xx02 ] || pc_fail "-s did not leave the three pieces"
pc_pass
