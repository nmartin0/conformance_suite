# With -p, a component longer than 14 bytes is diagnosed and one of 14 bytes is accepted.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pathchk.004
. "$PC_CORE/pc.sh"
pc_begin XCU.pathchk.004
pc_test
pathchk -p abcdefghijklmn >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "pathchk -p exited with status $rc for 14 bytes"
pathchk -p abcdefghijklmno >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "pathchk -p exited 0 for a component of 15 bytes"
[ -s err ] || pc_fail "no diagnostic"
pathchk -p a/abcdefghijklmno/b >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "pathchk -p exited 0 for a long middle component"
pc_pass
