# df -P writes a header line naming the columns with 512-byte blocks.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.df.001
. "$PC_CORE/pc.sh"
pc_begin XCU.df.001
pc_test
df -P . >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "df -P exited with status $rc"
set -- $(sed -n 1p out)
[ "$*" = "Filesystem 512-blocks Used Available Capacity Mounted on" ] || pc_fail "header is: $*"
pc_pass
