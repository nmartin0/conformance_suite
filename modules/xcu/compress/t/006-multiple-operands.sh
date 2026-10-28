# compress several files, zcat concatenates
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.compress.006
# "compress f1 f2" creates f1.Z and f2.Z and removes f1 and f2; "compress -d
# -c f1.Z f2.Z" writes the two decompressed files one after the other to
# standard output.
. "$PC_CORE/pc.sh"
pc_begin XCU.compress.006
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
i=0; while [ "$i" -lt 200 ]; do printf 'line %d of some repetitive test text\n' "$i"; i=$((i + 1)); done >orig || pc_unresolved "setup: cannot write input"
sed 's/line/LINE/' orig >orig2 || pc_unresolved "setup: cannot write second input"
cat orig >f1 && cat orig2 >f2 || pc_unresolved "setup: cannot copy inputs"
cat orig orig2 >exp || pc_unresolved "setup: cannot write expectation"
pc_test
compress f1 f2 </dev/null >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "compress f1 f2: exit status $rc"
[ -f f1.Z ] && [ -f f2.Z ] || pc_fail "f1.Z and f2.Z were not both created"
[ -e f1 ] || [ -e f2 ] && pc_fail "an input file was not removed"
compress -d -c f1.Z f2.Z </dev/null >both 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "compress -d -c: exit status $rc"
cmp -s both exp || pc_fail "output is not the two decompressed files in order"
pc_pass
