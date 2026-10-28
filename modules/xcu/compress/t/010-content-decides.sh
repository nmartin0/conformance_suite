# compress -d -c identifies the algorithm from the contents
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.compress.010
# "compress -d -c" on a file with an unrelated name (no suffix) decompresses
# both lzw data and gzip data correctly.
. "$PC_CORE/pc.sh"
pc_begin XCU.compress.010
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
i=0; while [ "$i" -lt 200 ]; do printf 'line %d of some repetitive test text\n' "$i"; i=$((i + 1)); done >orig || pc_unresolved "setup: cannot write input"
compress -c orig </dev/null >blob1 2>/dev/null || pc_unresolved "setup: compress -c failed"
compress -g -c orig </dev/null >blob2 2>/dev/null || pc_unresolved "setup: compress -g -c failed"
pc_test
compress -d -c blob1 </dev/null >o1 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "lzw data: exit status $rc"
cmp -s o1 orig || pc_fail "lzw data decompressed wrongly"
compress -d -c blob2 </dev/null >o2 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "gzip data: exit status $rc"
cmp -s o2 orig || pc_fail "gzip data decompressed wrongly"
pc_pass
