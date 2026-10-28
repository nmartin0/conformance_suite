# alias for a missing name fails with no output.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.alias.005
. "$PC_CORE/pc.sh"
pc_begin XCU.alias.005
pc_test
alias pcnosuch1 >out 2>/dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "alias for a missing name exited $rc"
[ ! -s out ] || pc_fail "alias for a missing name wrote to standard output"
pc_pass
