# param operands are parsed instead of the positional parameters.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.getopts.012
. "$PC_CORE/pc.sh"
pc_begin XCU.getopts.012
pc_test
set -- -Q
getopts ab pcopt -a -b x
rc1=$?
v1=$pcopt
getopts ab pcopt -a -b x
rc2=$?
v2=$pcopt
getopts ab pcopt -a -b x
rc3=$?
ind=$OPTIND
[ "$rc1" -eq 0 ] && [ "$v1" = a ] || pc_fail "first call: exit $rc1, name '$v1'"
[ "$rc2" -eq 0 ] && [ "$v2" = b ] || pc_fail "second call: exit $rc2, name '$v2'"
[ "$rc3" -eq 1 ] || pc_fail "third call exited $rc3, wanted 1"
[ "$ind" = 3 ] || pc_fail "OPTIND is '$ind', wanted 3"
[ "$1" = -Q ] || pc_fail "positional parameters were changed"
pc_pass
