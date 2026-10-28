# unlink of a file that does not exist gives a diagnostic and a non-zero status.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.unlink.002
. "$PC_CORE/pc.sh"
pc_begin XCU.unlink.002
pc_test
unlink nonexistent >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "unlink exited 0 for a missing file"
[ -s err ] || pc_fail "no diagnostic"
pc_pass
