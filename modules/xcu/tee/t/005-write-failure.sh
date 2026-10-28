# When a file operand cannot be written tee still copies to the other files and to standard output, and exits nonzero.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tee.005
. "$PC_CORE/pc.sh"
pc_begin XCU.tee.005
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'data\n' >in || pc_fail "cannot create input"
[ ! -e nodir ] || pc_unresolved "nodir exists"
pc_test
tee nodir/f good <in >out 2>/dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "status 0 although a file could not be written"
got=$(cat good; printf x)
[ "$got" = "data${NL}x" ] || pc_fail "the other file did not get the data"
got=$(cat out; printf x)
[ "$got" = "data${NL}x" ] || pc_fail "standard output did not get the data"
pc_pass
