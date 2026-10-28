# Global ordering options apply to keys; key options override them.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sort.015
. "$PC_CORE/pc.sh"
pc_begin XCU.sort.015
LC_ALL=POSIX
export LC_ALL
printf 'a 1\na 2\nb 1\n' >in1 || pc_unresolved "cannot create input"
printf 'a 10\na 9\nb 1\n' >in2 || pc_unresolved "cannot create input"
pc_test
sort -r -k1,1 -k2,2 in1 >out 2>err </dev/null || pc_fail "sort -r -k1,1 -k2,2: nonzero status"
printf 'b 1\na 2\na 1\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "global -r was not applied to both keys: $(cat out)"
sort -r -k1,1 -k2,2n in2 >out 2>err </dev/null || pc_fail "sort -r -k1,1 -k2,2n: nonzero status"
printf 'b 1\na 9\na 10\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "the key option n did not override the global -r: $(cat out)"
pc_pass
