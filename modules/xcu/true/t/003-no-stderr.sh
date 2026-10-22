# true writes nothing to standard error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.true.003
# MUTANT    true.stderr
# MUTANT    true.exec_stderr
#
# Each check runs twice: the way the shell runs the name (often a
# built-in) and by a direct exec (XCU 1.6 requires both to work).
. "$PC_CORE/pc.sh"
pc_begin XCU.true.003
pc_test
true 2>err
[ -s err ] && pc_fail "true wrote to standard error"
(exec true) 2>err2
[ -s err2 ] && pc_fail "exec true wrote to standard error"
pc_pass
