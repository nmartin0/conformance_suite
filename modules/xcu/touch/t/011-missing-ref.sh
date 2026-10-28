# touch -r with a reference file that does not exist fails with a diagnostic and a non-zero status.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.touch.011
. "$PC_CORE/pc.sh"
pc_begin XCU.touch.011
pc_test
touch -r nonexistent f >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "touch exited 0 for a missing reference file"
[ -s err ] || pc_fail "no diagnostic"
pc_pass
