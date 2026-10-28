# getopts returns each flag in turn and then ends with exit 1.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.getopts.002
. "$PC_CORE/pc.sh"
pc_begin XCU.getopts.002
pc_test
set -- -a -b file
getopts ab pcopt
rc1=$?
v1=$pcopt
getopts ab pcopt
rc2=$?
v2=$pcopt
getopts ab pcopt
rc3=$?
v3=$pcopt
ind=$OPTIND
[ "$rc1" -eq 0 ] && [ "$v1" = a ] || pc_fail "first call: exit $rc1, name '$v1'"
[ "$rc2" -eq 0 ] && [ "$v2" = b ] || pc_fail "second call: exit $rc2, name '$v2'"
[ "$rc3" -eq 1 ] || pc_fail "third call exited $rc3, wanted 1"
[ "$v3" = '?' ] || pc_fail "name is '$v3' at the end of options, wanted ?"
[ "$ind" = 3 ] || pc_fail "OPTIND is '$ind' at the end of options, wanted 3"
pc_pass
