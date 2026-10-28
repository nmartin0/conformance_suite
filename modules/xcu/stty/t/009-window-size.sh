# window size: rows, cols and size
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.stty.009
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.stty.009
pc_pty_need
cat >inner.sh <<'IN'
stty rows 31 cols 97 >set.out 2>set.err; echo $? >set.rc
stty size >size1.out
stty rows 12 >r.out; stty size >size2.out
stty cols 40 >c.out; stty size >size3.out
IN
pc_test
pc_pty_run inner.sh
[ "$(cat set.rc 2>/dev/null)" = 0 ] || pc_fail "stty rows 31 cols 97 failed"
[ ! -s set.out ] || pc_fail "an operand that is not a query wrote output"
[ "$(cat size1.out)" = "31 97" ] || pc_fail "size after rows 31 cols 97 was '$(cat size1.out)'"
[ "$(cat size2.out)" = "12 97" ] || pc_fail "size after rows 12 was '$(cat size2.out)'"
[ "$(cat size3.out)" = "12 40" ] || pc_fail "size after cols 40 was '$(cat size3.out)'"
pc_pass
