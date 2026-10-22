# false writes nothing to standard error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.false.003
# MUTANT    false.stderr
# MUTANT    false.exec_stderr
#
# Each check runs twice: the way the shell runs the name (often a
# built-in) and by a direct exec (XCU 1.6 requires both to work).
. "$PC_CORE/pc.sh"
pc_begin XCU.false.003
pc_test
false 2>err
[ -s err ] && pc_fail "false wrote to standard error"
(exec false) 2>err2
[ -s err2 ] && pc_fail "exec false wrote to standard error"
pc_pass
