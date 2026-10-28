# If file1 does not exist, link fails with a diagnostic and creates nothing.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.link.003
. "$PC_CORE/pc.sh"
pc_begin XCU.link.003
printf 'x\n' >a || pc_fail "cannot create a"
ln a probe || pc_unresolved "setup: hard links are not available here"
pc_test
link nonexistent b >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "link exited 0 for a missing file1"
[ -s err ] || pc_fail "no diagnostic"
[ ! -e b ] || pc_fail "b was created"
pc_pass
