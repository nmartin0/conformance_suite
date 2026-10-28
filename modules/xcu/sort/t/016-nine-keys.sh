# Nine -k options are accepted and significant in command-line order.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sort.016
. "$PC_CORE/pc.sh"
pc_begin XCU.sort.016
LC_ALL=POSIX
export LC_ALL
printf 'a a a a a a a a z\na a a a a a a a y\n' >in1 || pc_unresolved "cannot create input"
printf 'a b\nb a\n' >in2 || pc_unresolved "cannot create input"
pc_test
sort -k1,1 -k2,2 -k3,3 -k4,4 -k5,5 -k6,6 -k7,7 -k8,8 -k9,9 in1 >out 2>err </dev/null || pc_fail "sort with nine keys: nonzero status: $(cat err)"
printf 'a a a a a a a a y\na a a a a a a a z\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "the ninth key was not significant: $(cat out)"
sort -k2,2 -k1,1 in2 >out 2>err </dev/null || pc_fail "sort -k2,2 -k1,1: nonzero status"
printf 'b a\na b\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "the keys were not applied in command-line order: $(cat out)"
pc_pass
