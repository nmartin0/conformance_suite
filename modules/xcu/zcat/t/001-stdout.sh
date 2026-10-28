# zcat writes the decompressed data
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.zcat.001
# "zcat f.Z" writes the original bytes to standard output, leaves f.Z
# present and unchanged, and creates no f.
. "$PC_CORE/pc.sh"
pc_begin XCU.zcat.001
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
i=0; while [ "$i" -lt 200 ]; do printf 'line %d of some repetitive test text\n' "$i"; i=$((i + 1)); done >orig || pc_unresolved "setup: cannot write input"
cat orig >f || pc_unresolved "setup: cannot copy input"
compress f </dev/null >/dev/null 2>&1 || pc_unresolved "setup: compress failed"
cat f.Z >f.keep || pc_unresolved "setup: cannot copy the compressed file"
pc_test
zcat f.Z </dev/null >o1 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "zcat f.Z: exit status $rc"
cmp -s o1 orig || pc_fail "zcat f.Z: output differs from the original"
cmp -s f.Z f.keep || pc_fail "zcat changed or removed f.Z"
[ -e f ] && pc_fail "zcat created f"
pc_pass
