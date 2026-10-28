# -- ends the options.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.getopts.005
. "$PC_CORE/pc.sh"
pc_begin XCU.getopts.005
pc_test
set -- -a -- -b
getopts ab pcopt
rc1=$?
getopts ab pcopt
rc2=$?
v2=$pcopt
ind=$OPTIND
[ "$rc1" -eq 0 ] || pc_fail "first call exited $rc1"
[ "$rc2" -eq 1 ] || pc_fail "call at -- exited $rc2, wanted 1"
[ "$v2" = '?' ] || pc_fail "name is '$v2' at --, wanted ?"
[ "$ind" = 3 ] || pc_fail "OPTIND is '$ind' after --, wanted 3"
pc_pass
