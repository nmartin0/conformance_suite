# compress with no operands and with -
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.compress.009
# With no file operand, and with the operand -, compress reads standard
# input and writes standard output; "compress -d" of the result restores the
# original bytes.
. "$PC_CORE/pc.sh"
pc_begin XCU.compress.009
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
i=0; while [ "$i" -lt 200 ]; do printf 'line %d of some repetitive test text\n' "$i"; i=$((i + 1)); done >orig || pc_unresolved "setup: cannot write input"
pc_test
compress <orig >c1 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "compress with no operand: exit status $rc"
compress - <orig >c2 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "compress -: exit status $rc"
compress -d <c1 >b1 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "compress -d with no operand: exit status $rc"
cmp -s b1 orig || pc_fail "no-operand round trip differs"
compress -d - <c2 >b2 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "compress -d -: exit status $rc"
cmp -s b2 orig || pc_fail "round trip with - differs"
pc_pass
