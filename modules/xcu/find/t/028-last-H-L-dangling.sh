# find: last of -H/-L wins, dangling links are the link itself
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.find.028
. "$PC_CORE/pc.sh"
pc_begin XCU.find.028
mkdir d || pc_unresolved "cannot create d"
printf 'x\n' >d/f || pc_unresolved "cannot create d/f"
ln -s nowhere d/dang || pc_unresolved "cannot create symbolic link"
ln -s f d/lf || pc_unresolved "cannot create symbolic link"
ln -s d/f top || pc_unresolved "cannot create symbolic link"
pc_test
# -H -L : -L wins, so the link top is followed to a regular file
got=$(find -H -L top -type f 2>err </dev/null) || pc_fail "find -H -L: nonzero status: $(cat err)"
[ "$got" = top ] || pc_fail "-H -L: last option did not win: '$got'"
# -L -H : -H wins, so links inside are not followed
find -L -H d -type l >out 2>err </dev/null || pc_fail "find -L -H: nonzero status: $(cat err)"
got=$(sort out | tr '\n' ' ')
[ "$got" = 'd/dang d/lf ' ] || pc_fail "-L -H: last option did not win: '$got'"
# -H -L : -L wins, so d/lf is followed, but the dangling link is the link itself
find -H -L d -type l >out 2>err </dev/null || pc_fail "find -H -L d: nonzero status: $(cat err)"
got=$(sort out | tr '\n' ' ')
[ "$got" = 'd/dang ' ] || pc_fail "-H -L: wrong set of links: '$got'"
got=$(find -H d/dang -type l 2>err </dev/null) || pc_fail "find -H dangling: nonzero status: $(cat err)"
[ "$got" = d/dang ] || pc_fail "-H: a dangling operand link is not the link itself: '$got'"
pc_pass
