# Options grouped in one word are returned in order.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.getopts.004
. "$PC_CORE/pc.sh"
pc_begin XCU.getopts.004
pc_test
set -- -ab x
getopts ab pcopt
rc1=$?
v1=$pcopt
getopts ab pcopt
rc2=$?
v2=$pcopt
getopts ab pcopt
rc3=$?
ind=$OPTIND
[ "$rc1" -eq 0 ] && [ "$v1" = a ] || pc_fail "first of -ab: exit $rc1, name '$v1'"
[ "$rc2" -eq 0 ] && [ "$v2" = b ] || pc_fail "second of -ab: exit $rc2, name '$v2'"
[ "$rc3" -eq 1 ] || pc_fail "third call exited $rc3, wanted 1"
[ "$ind" = 2 ] || pc_fail "OPTIND is '$ind' at the end, wanted 2"
pc_pass
