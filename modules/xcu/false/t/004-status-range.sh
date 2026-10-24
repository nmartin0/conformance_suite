# false exits with a status from 1 to 125.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.false.004
# MUTANT    false.exit126
# MUTANT    false.exec_exit126
#
# Each check runs twice: the way the shell runs the name (often a
# built-in) and by a direct exec (XCU 1.6 requires both to work).
. "$PC_CORE/pc.sh"
pc_begin XCU.false.004
pc_test
false
rc=$?
if [ "$rc" -lt 1 ] || [ "$rc" -gt 125 ]; then
	pc_fail "false exited with status $rc"
fi
(exec false)
rc=$?
if [ "$rc" -lt 1 ] || [ "$rc" -gt 125 ]; then
	pc_fail "exec false exited with status $rc"
fi
pc_pass
