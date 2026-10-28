# zcat from standard input
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.zcat.003
# With no operand, and with the operand -, zcat decompresses standard input
# to standard output.
. "$PC_CORE/pc.sh"
pc_begin XCU.zcat.003
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
i=0; while [ "$i" -lt 200 ]; do printf 'line %d of some repetitive test text\n' "$i"; i=$((i + 1)); done >orig || pc_unresolved "setup: cannot write input"
compress <orig >c.Z 2>/dev/null || pc_unresolved "setup: compress failed"
pc_test
zcat <c.Z >o1 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "zcat with no operand: exit status $rc"
cmp -s o1 orig || pc_fail "no-operand output differs from the original"
zcat - <c.Z >o2 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "zcat -: exit status $rc"
cmp -s o2 orig || pc_fail "output with - differs from the original"
pc_pass
