# false writes nothing to standard output.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.false.002
# MUTANT    false.prints_newline
# MUTANT    false.exec_prints_newline
#
# Each check runs twice: the way the shell runs the name (often a
# built-in) and by a direct exec (XCU 1.6 requires both to work).
. "$PC_CORE/pc.sh"
pc_begin XCU.false.002
pc_test
false >out
[ -s out ] && pc_fail "false wrote to standard output"
(exec false) >out2
[ -s out2 ] && pc_fail "exec false wrote to standard output"
pc_pass
