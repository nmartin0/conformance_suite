# command -v for an unknown name writes nothing and fails.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.command.007
. "$PC_CORE/pc.sh"
pc_begin XCU.command.007
pc_test
command -v pcnosuch5 >out 2>/dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "command -v exited $rc for an unknown name"
[ ! -s out ] || pc_fail "command -v wrote to standard output for an unknown name"
pc_pass
