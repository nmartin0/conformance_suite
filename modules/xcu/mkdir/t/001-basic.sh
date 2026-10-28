# mkdir creates the named directories in the order given, with no output; a later operand may live inside an earlier one.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mkdir.001
. "$PC_CORE/pc.sh"
pc_begin XCU.mkdir.001
pc_test
mkdir d1 d2 x x/y >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "mkdir exited with status $rc"
[ -d d1 ] && [ -d d2 ] && [ -d x ] || pc_fail "a directory was not created"
[ -d x/y ] || pc_fail "x/y was not created (operands out of order?)"
[ ! -s out ] || pc_fail "mkdir wrote to standard output"
pc_pass
