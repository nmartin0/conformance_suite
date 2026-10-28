# Without -H or -S a new limit sets both limits.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ulimit.005
. "$PC_CORE/pc.sh"
pc_begin XCU.ulimit.005
h0=$(ulimit -H -f 2>/dev/null) || pc_fail "ulimit -H -f failed"
[ "$h0" != 0 ] || pc_unresolved "the hard file size limit is already 0, so setting both cannot be told from setting the soft limit"
pc_test
out=$( ulimit -f 0 2>/dev/null && ulimit -S -f && ulimit -H -f )
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ulimit -f 0 failed (exit $rc)"
{
	IFS= read -r soft
	IFS= read -r hard
} <<PCX
$out
PCX
[ "$soft" = 0 ] || pc_fail "the soft limit is '$soft', wanted 0"
[ "$hard" = 0 ] || pc_fail "the hard limit is '$hard', wanted 0"
pc_pass
