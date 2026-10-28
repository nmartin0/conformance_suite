# df -k -P writes the header with 1024-byte blocks.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.df.002
. "$PC_CORE/pc.sh"
pc_begin XCU.df.002
pc_test
df -k -P . >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "df -k -P exited with status $rc"
set -- $(sed -n 1p out)
[ "$*" = "Filesystem 1024-blocks Used Available Capacity Mounted on" ] || pc_fail "header is: $*"
pc_pass
