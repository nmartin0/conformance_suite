# Integer values in the shell's arithmetic, in expr and in test cover at least the range of a signed C long of 32 bits: 2147483647 and -2147483648 can be produced, printed and compared
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.1-relationship.012
. "$PC_CORE/pc.sh"
pc_begin XCU.1.1-relationship.012
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
pc_test
[ "$((2147483646 + 1))" = 2147483647 ] || pc_fail "sh: 2147483646 + 1 is $((2147483646 + 1))"
[ "$((-2147483647 - 1))" = -2147483648 ] || pc_fail "sh: -2147483647 - 1 is $((-2147483647 - 1))"
[ "$((65535 * 32768 + 32767))" = 2147483647 ] || pc_fail "sh: 65535 * 32768 + 32767 is $((65535 * 32768 + 32767))"
[ "$((2147483647 > 2147483646))" = 1 ] || pc_fail "sh: comparison of large values"
x=2147483647
[ "$((x + 0))" = 2147483647 ] || pc_fail "sh: a variable holding 2147483647"
r=$(expr 2147483646 + 1) || pc_fail "expr failed"
[ "$r" = 2147483647 ] || pc_fail "expr: 2147483646 + 1 is $r"
r=$(expr 0 - 2147483647 - 1) || pc_fail "expr failed"
[ "$r" = -2147483648 ] || pc_fail "expr: 0 - 2147483647 - 1 is $r"
r=$(expr 2147483647 \> 2147483646)
[ "$r" = 1 ] || pc_fail "expr: comparison of large values is $r"
[ 2147483647 -gt 2147483646 ] || pc_fail "test: -gt on large values"
[ -2147483648 -lt -2147483647 ] || pc_fail "test: -lt on large negative values"
[ 2147483647 -eq 2147483647 ] || pc_fail "test: -eq on large values"
pc_pass
