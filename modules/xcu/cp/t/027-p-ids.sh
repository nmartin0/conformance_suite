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
set -- $b; bm=$1; bo=$3; bg=$4
[ "$bo" = 1234 ] && [ "$bg" = 2345 ] || pc_unresolved "the owner and group of src are '$bo' and '$bg', not those just set"
case $bm in
???s??s*) ;;
*) pc_unresolved "src has mode '$bm', not set-user-ID and set-group-ID" ;;
esac
pc_test
cp -p src dst >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cp -p exited with status $rc"
[ -f dst ] || pc_fail "cp -p did not create dst"
a=$(ls -n dst) || pc_fail "cannot list dst"
set -- $a; am=$1; ao=$3; ag=$4
[ "$ao" = 1234 ] && [ "$ag" = 2345 ] || pc_fail "the copy has owner '$ao' and group '$ag', expected 1234 and 2345"
[ "$ao" = "$bo" ] || pc_fail "owner $ao differs from $bo"
[ "$ag" = "$bg" ] || pc_fail "group $ag differs from $bg"
[ "$am" = "$bm" ] || pc_fail "mode $am differs from $bm"
pc_pass
