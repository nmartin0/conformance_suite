# test writes nothing to standard output, for a true and for a false result.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.test.016
# MUTANT    test.stdout_true
# MUTANT    test.stdout_false
# MUTANT    test.stdout_newline
# MUTANT    test.fn_stdout_true
# MUTANT    test.te_stdout_true
# MUTANT    test.br_stdout_true
#
# Output goes to a file, not a command substitution, which would hide a
# lone newline.
. "$PC_CORE/pc.sh"
pc_begin XCU.test.016
pc_test
test a = a >out
[ -s out ] && pc_fail "test with a true result wrote to standard output"
test a = b >out
[ -s out ] && pc_fail "test with a false result wrote to standard output"
test >out
[ -s out ] && pc_fail "test with no argument wrote to standard output"
[ a = a ] >out
[ -s out ] && pc_fail "[ with a true result wrote to standard output"
[ a = b ] >out
[ -s out ] && pc_fail "[ with a false result wrote to standard output"
(exec test a = a) >out
[ -s out ] && pc_fail "exec test with a true result wrote to standard output"
(exec test a = b) >out
[ -s out ] && pc_fail "exec test with a false result wrote to standard output"
(exec [ a = a ]) >out
[ -s out ] && pc_fail "exec [ with a true result wrote to standard output"
(exec [ a = b ]) >out
[ -s out ] && pc_fail "exec [ with a false result wrote to standard output"
pc_pass
