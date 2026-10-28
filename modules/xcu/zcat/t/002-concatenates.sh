# zcat of several files
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.zcat.002
# "zcat f1.Z f2.Z" writes the decompressed data of f1.Z then that of f2.Z.
. "$PC_CORE/pc.sh"
pc_begin XCU.zcat.002
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
i=0; while [ "$i" -lt 200 ]; do printf 'line %d of some repetitive test text\n' "$i"; i=$((i + 1)); done >orig || pc_unresolved "setup: cannot write input"
sed 's/line/LINE/' orig >orig2 || pc_unresolved "setup: cannot write second input"
cat orig >f1 && cat orig2 >f2 || pc_unresolved "setup: cannot copy inputs"
compress f1 f2 </dev/null >/dev/null 2>&1 || pc_unresolved "setup: compress failed"
cat orig orig2 >exp || pc_unresolved "setup: cannot write expectation"
pc_test
zcat f1.Z f2.Z </dev/null >o 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "exit status $rc"
cmp -s o exp || pc_fail "output is not the two decompressed files in order"
pc_pass
