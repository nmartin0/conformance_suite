# renice of a process that does not exist.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.renice.004
. "$PC_CORE/pc.sh"
pc_begin XCU.renice.004
LC_ALL=C; export LC_ALL
pc_test
renice -n 1 2147483646 >out 2>err </dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "renice of a nonexistent process exited with status 0"
renice -n 1 -p 2147483646 >out 2>err </dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "renice -p of a nonexistent process exited with status 0"
pc_pass
