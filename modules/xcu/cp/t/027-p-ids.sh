# cp -p preserves owner, group and set-id bits.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cp.027
. "$PC_CORE/pc.sh"
pc_begin XCU.cp.027
printf 'x\n' >src || pc_unresolved "cannot create src"
chown 1234:2345 src 2>/dev/null || pc_unresolved "cannot change owner"
chmod 6755 src || pc_unresolved "cannot set mode"
b=$(ls -n src) || pc_unresolved "cannot list src"
pc_test
cp -p src dst >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cp -p exited with status $rc"
a=$(ls -n dst) || pc_fail "cannot list dst"
set -- $b; bm=$1; bo=$3; bg=$4
set -- $a; am=$1; ao=$3; ag=$4
[ "$ao" = "$bo" ] || pc_fail "owner $ao differs from $bo"
[ "$ag" = "$bg" ] || pc_fail "group $ag differs from $bg"
[ "$am" = "$bm" ] || pc_fail "mode $am differs from $bm"
pc_pass
