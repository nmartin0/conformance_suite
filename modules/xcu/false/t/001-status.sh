# false exits with a non-zero status.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.false.001
# MUTANT    false.exit0
# MUTANT    false.exec_exit0
#
# Each check runs twice: the way the shell runs the name (often a
# built-in) and by a direct exec (XCU 1.6 requires both to work).
. "$PC_CORE/pc.sh"
pc_begin XCU.false.001
pc_test
false
rc=$?
if [ "$rc" -eq 0 ]; then
	pc_fail "false exited with status 0"
fi
(exec false)
rc=$?
if [ "$rc" -eq 0 ]; then
	pc_fail "exec false exited with status 0"
fi
pc_pass
