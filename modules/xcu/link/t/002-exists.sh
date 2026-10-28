# If file2 exists, link fails with a diagnostic and file2 is not replaced.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.link.002
. "$PC_CORE/pc.sh"
pc_begin XCU.link.002
printf 'x\n' >a || pc_fail "cannot create a"
printf 'y\n' >b || pc_fail "cannot create b"
ln a probe || pc_unresolved "setup: hard links are not available here"
pc_test
link a b >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "link exited 0 although b exists"
[ -s err ] || pc_fail "no diagnostic"
[ ! a -ef b ] || pc_fail "b was replaced"
pc_pass
