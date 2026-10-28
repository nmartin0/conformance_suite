# An unsupported option is an error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uname.005
. "$PC_CORE/pc.sh"
pc_begin XCU.uname.005
LC_ALL=C; export LC_ALL
pc_test
uname -Z >out 2>err </dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "uname -Z exited with status 0"
pc_pass
