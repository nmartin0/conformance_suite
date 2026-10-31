# Without -H or -S a new limit sets both limits.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ulimit.005
. "$PC_CORE/pc.sh"
pc_begin XCU.ulimit.005
h0=$(ulimit -H -f 2>/dev/null) || pc_fail "ulimit -H -f failed"
# A small but not zero limit: a shell may keep the output of a command
# substitution in a temporary file, which a limit of 0 would make it unable to write.
n=100
case $h0 in
unlimited) ;;
''|*[!0-9]*) pc_unresolved "the hard file size limit '$h0' is not a number" ;;
*) [ "$h0" -gt "$n" ] || pc_unresolved "the hard file size limit $h0 is too low for this test" ;;
esac
pc_test
out=$( ulimit -f $n 2>/dev/null && ulimit -S -f && ulimit -H -f )
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ulimit -f $n failed (exit $rc)"
{
	IFS= read -r soft
	IFS= read -r hard
} <<PCX
$out
PCX
[ "$soft" = "$n" ] || pc_fail "the soft limit is '$soft', wanted $n"
[ "$hard" = "$n" ] || pc_fail "the hard limit is '$hard', wanted $n"
pc_pass
