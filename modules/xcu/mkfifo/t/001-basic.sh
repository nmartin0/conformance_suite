# mkfifo creates a FIFO for each operand, in order, with no output.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mkfifo.001
. "$PC_CORE/pc.sh"
pc_begin XCU.mkfifo.001
pc_test
mkfifo f1 f2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "mkfifo exited with status $rc"
[ -p f1 ] && [ -p f2 ] || pc_fail "an operand is not a FIFO"
[ ! -s out ] || pc_fail "mkfifo wrote to standard output"
pc_pass
