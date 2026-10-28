# command removes the special property of a special built-in.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.command.002
. "$PC_CORE/pc.sh"
pc_begin XCU.command.002
unset pcv
pc_test
pcv=set command : 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "command : exited $rc"
[ "${pcv+x}" != x ] || pc_fail "an assignment before command : stayed in the shell"
pc_pass
