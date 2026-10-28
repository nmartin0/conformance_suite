# The command name given to command is not alias-substituted.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.command.012
. "$PC_CORE/pc.sh"
pc_begin XCU.command.012
alias pcl9='echo ALIASED'
pc_test
got=$(eval 'command pcl9' 2>/dev/null)
rc=$?
[ "$got" != ALIASED ] || pc_fail "the operand of command was substituted as an alias"
[ "$rc" -eq 127 ] || pc_fail "exit $rc, wanted 127 (no such utility)"
pc_pass
