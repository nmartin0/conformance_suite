# df of a file that does not exist fails with a diagnostic.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.df.007
. "$PC_CORE/pc.sh"
pc_begin XCU.df.007
pc_test
df -P nonexistent >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "df exited 0 for a missing file"
[ -s err ] || pc_fail "no diagnostic"
pc_pass
