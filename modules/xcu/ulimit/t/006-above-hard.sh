# A soft limit above the hard limit is refused.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ulimit.006
. "$PC_CORE/pc.sh"
pc_begin XCU.ulimit.006
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
out=$( ulimit -f $n 2>/dev/null; ulimit -S -f $((n + 1)) 2>/dev/null; echo "rc=$?"; ulimit -S -f )
{
	IFS= read -r rcline
	IFS= read -r soft
} <<PCX
$out
PCX
case $rcline in
rc=0) pc_fail "raising the soft limit above the hard limit exited 0" ;;
rc=*) ;;
*) pc_fail "setup: unexpected output '$out'" ;;
esac
[ "$soft" = "$n" ] || pc_fail "the soft limit is '$soft' after the refused request, wanted $n"
pc_pass
