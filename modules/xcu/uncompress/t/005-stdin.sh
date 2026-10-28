# uncompress with no operands
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uncompress.005
# With no file operand uncompress reads compressed data from standard input
# and writes the original bytes to standard output.
. "$PC_CORE/pc.sh"
pc_begin XCU.uncompress.005
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
i=0; while [ "$i" -lt 200 ]; do printf 'line %d of some repetitive test text\n' "$i"; i=$((i + 1)); done >orig || pc_unresolved "setup: cannot write input"
compress <orig >c.Z 2>/dev/null || pc_unresolved "setup: compress failed"
pc_test
uncompress <c.Z >o 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "exit status $rc"
cmp -s o orig || pc_fail "standard output differs from the original"
pc_pass
