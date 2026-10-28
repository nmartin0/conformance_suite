# command -V for an unknown name fails.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.command.009
. "$PC_CORE/pc.sh"
pc_begin XCU.command.009
pc_test
command -V pcnosuch6 >/dev/null 2>&1
rc=$?
[ "$rc" -gt 0 ] || pc_fail "command -V exited $rc for an unknown name"
pc_pass
