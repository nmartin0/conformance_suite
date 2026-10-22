# true exits with status zero.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.true.001
# MUTANT    true.exit1
# MUTANT    true.exec_exit1
#
# Each check runs twice: the way the shell runs the name (often a
# built-in) and by a direct exec (XCU 1.6 requires both to work).
. "$PC_CORE/pc.sh"
pc_begin XCU.true.001
pc_test
true
rc=$?
[ "$rc" -eq 0 ] || pc_fail "true exited with status $rc"
(exec true)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "exec true exited with status $rc"
pc_pass
