# type for an unknown name fails.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.type.002
. "$PC_CORE/pc.sh"
pc_begin XCU.type.002
pc_test
type pcnosuch8 >/dev/null 2>&1
rc=$?
[ "$rc" -gt 0 ] || pc_fail "type of an unknown name exited $rc"
pc_pass
