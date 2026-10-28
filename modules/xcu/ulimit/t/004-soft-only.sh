# ulimit -S -f n lowers the soft limit and leaves the hard limit.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ulimit.004
. "$PC_CORE/pc.sh"
pc_begin XCU.ulimit.004
h0=$(ulimit -H -f 2>/dev/null) || pc_fail "ulimit -H -f failed"
pc_test
out=$( ulimit -S -f 0 2>/dev/null && ulimit -S -f && ulimit -H -f )
rc=$?
[ "$rc" -eq 0 ] || pc_fail "setting the soft limit failed (exit $rc)"
{
	IFS= read -r soft
	IFS= read -r hard
} <<PCX
$out
PCX
[ "$soft" = 0 ] || pc_fail "the soft limit reads back as '$soft', wanted 0"
[ "$hard" = "$h0" ] || pc_fail "the hard limit is '$hard', it was '$h0'"
pc_pass
