# true writes nothing to standard output.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.true.002
# MUTANT    true.prints
# MUTANT    true.prints_newline
# MUTANT    true.exec_prints_newline
#
# Each check runs twice: the way the shell runs the name (often a
# built-in) and by a direct exec (XCU 1.6 requires both to work).
#
# Output goes to a file, not a command substitution, which would hide a
# lone newline.
. "$PC_CORE/pc.sh"
pc_begin XCU.true.002
pc_test
true >out
[ -s out ] && pc_fail "true wrote to standard output"
(exec true) >out2
[ -s out2 ] && pc_fail "exec true wrote to standard output"
pc_pass
