# With no operand du reports the current directory.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.du.010
. "$PC_CORE/pc.sh"
pc_begin XCU.du.010
mkdir d || pc_fail "cannot create d"
printf 'abc\n' >d/a || pc_fail "cannot create d/a"
pc_test
(cd d && du -s) >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "du exited with status $rc"
set -- $(cat out)
[ "$#" -eq 2 ] && [ "$2" = . ] || pc_fail "output is: $*"
pc_pass
