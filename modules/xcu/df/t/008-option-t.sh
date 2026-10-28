# df -t is accepted and writes output (its format is unspecified).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.df.008
. "$PC_CORE/pc.sh"
pc_begin XCU.df.008
pc_test
df -t . >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "df -t exited with status $rc"
[ -s out ] || pc_fail "df -t wrote nothing"
pc_pass
