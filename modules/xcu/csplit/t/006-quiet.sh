# csplit -s writes no size lines.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.csplit.006
. "$PC_CORE/pc.sh"
pc_begin XCU.csplit.006
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '%s\n' 1 2 3 4 5 6 7 8 9 10 >in10 || pc_fail "cannot create input"
pc_test
csplit -s in10 5 >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "csplit -s gave status $rc"
[ -s out ] && pc_fail "-s still wrote to standard output"
[ -s xx00 ] && [ -s xx01 ] || pc_fail "-s did not leave the pieces"
pc_pass
