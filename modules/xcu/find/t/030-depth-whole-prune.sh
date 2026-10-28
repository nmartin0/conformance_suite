# find: -depth applies to the whole expression; -prune has no effect with -depth
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.find.030
. "$PC_CORE/pc.sh"
pc_begin XCU.find.030
mkdir -p d/sub || pc_unresolved "cannot create d/sub"
printf 'x\n' >d/sub/f || pc_unresolved "cannot create d/sub/f"
pc_test
# -depth is inside an operand that is never evaluated
find d -name zzz -a -depth -o -print >out 2>err </dev/null || pc_fail "find: nonzero status: $(cat err)"
printf 'd/sub/f\nd/sub\nd\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "-depth did not apply to the whole expression: $(cat out)"
# with -depth, -prune has no effect: the contents of sub are still visited
find d -depth -name sub -prune -o -print >out 2>err </dev/null || pc_fail "find -depth -prune: nonzero status: $(cat err)"
grep -x 'd/sub/f' out >/dev/null || pc_fail "-prune stopped the descent although -depth was given: $(cat out)"
grep -x 'd/sub' out >/dev/null && pc_fail "d/sub was printed although -prune was true for it"
# without -depth it does prune
find d -name sub -prune -o -print >out 2>err </dev/null || pc_fail "find -prune: nonzero status"
grep -x 'd/sub/f' out >/dev/null && pc_fail "-prune did not stop the descent without -depth"
pc_pass
