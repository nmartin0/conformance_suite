# A soft limit above the hard limit is refused.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ulimit.006
. "$PC_CORE/pc.sh"
pc_begin XCU.ulimit.006
pc_test
out=$( ulimit -f 0 2>/dev/null; ulimit -S -f 1 2>/dev/null; echo "rc=$?"; ulimit -S -f )
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
[ "$soft" = 0 ] || pc_fail "the soft limit is '$soft' after the refused request, wanted 0"
pc_pass
