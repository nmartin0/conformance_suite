# who: status 0, quiet standard error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.who.001
. "$PC_CORE/pc.sh"
pc_begin XCU.who.001
LC_ALL=C; export LC_ALL
pc_test
who >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "who exited with status $rc"
[ -s err ] && pc_fail "who wrote to standard error"
pc_pass
