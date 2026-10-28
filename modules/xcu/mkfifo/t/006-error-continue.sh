# If one FIFO cannot be created, mkfifo diagnoses it, creates the others, and exits non-zero.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mkfifo.006
. "$PC_CORE/pc.sh"
pc_begin XCU.mkfifo.006
pc_test
mkfifo f1 nodir/x f2 >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "mkfifo exited 0 although nodir does not exist"
[ -s err ] || pc_fail "no diagnostic"
[ -p f1 ] && [ -p f2 ] || pc_fail "the other FIFOs were not created"
pc_pass
