# stty without operands succeeds and reports
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.stty.003
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.stty.003
pc_pty_need
cat >inner.sh <<'IN'
stty >d.out 2>d.err
echo $? >d.rc
IN
pc_test
pc_pty_run inner.sh
[ "$(cat d.rc 2>/dev/null)" = 0 ] || pc_fail "stty without operands failed"
[ -s d.out ] || pc_fail "stty without operands wrote nothing"
pc_pass
