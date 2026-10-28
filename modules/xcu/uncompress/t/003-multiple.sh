# uncompress several files
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uncompress.003
# "uncompress f1.Z f2.Z" restores f1 and f2.
. "$PC_CORE/pc.sh"
pc_begin XCU.uncompress.003
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
i=0; while [ "$i" -lt 200 ]; do printf 'line %d of some repetitive test text\n' "$i"; i=$((i + 1)); done >orig || pc_unresolved "setup: cannot write input"
sed 's/line/LINE/' orig >orig2 || pc_unresolved "setup: cannot write second input"
cat orig >f1 && cat orig2 >f2 || pc_unresolved "setup: cannot copy inputs"
compress f1 f2 </dev/null >/dev/null 2>&1 || pc_unresolved "setup: compress failed"
pc_test
uncompress f1.Z f2.Z </dev/null >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "exit status $rc"
cmp -s f1 orig || pc_fail "f1 differs from the original"
cmp -s f2 orig2 || pc_fail "f2 differs from the original"
pc_pass
