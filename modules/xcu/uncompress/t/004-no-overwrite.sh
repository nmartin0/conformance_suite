# uncompress refuses to overwrite without -f
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uncompress.004
# With f already present, standard input not a terminal and no -f,
# "uncompress f.Z" exits nonzero and leaves f unchanged; with -f f is
# replaced by the decompressed data.
. "$PC_CORE/pc.sh"
pc_begin XCU.uncompress.004
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
i=0; while [ "$i" -lt 200 ]; do printf 'line %d of some repetitive test text\n' "$i"; i=$((i + 1)); done >orig || pc_unresolved "setup: cannot write input"
cat orig >f || pc_unresolved "setup: cannot copy input"
compress f </dev/null >/dev/null 2>&1 || pc_unresolved "setup: compress failed"
printf 'present' >f || pc_unresolved "setup: cannot write existing file"
pc_test
uncompress f.Z </dev/null >out 2>err
rc=$?
[ "$rc" -ne 0 ] || pc_fail "overwriting f without -f gave status 0"
[ "$(cat f)" = present ] || pc_fail "the existing f was overwritten"
uncompress -f f.Z </dev/null >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "uncompress -f: exit status $rc"
cmp -s f orig || pc_fail "f was not replaced by the decompressed data"
pc_pass
