# compress -c
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.compress.002
# "compress -c f" writes the compressed data to standard output, leaves f
# unchanged and creates no f.Z; "compress -d -c" of that output read from
# standard input gives back the original bytes.
. "$PC_CORE/pc.sh"
pc_begin XCU.compress.002
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
i=0; while [ "$i" -lt 200 ]; do printf 'line %d of some repetitive test text\n' "$i"; i=$((i + 1)); done >orig || pc_unresolved "setup: cannot write input"
cat orig >f || pc_unresolved "setup: cannot copy input"
pc_test
compress -c f </dev/null >c.out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "compress -c: exit status $rc"
cmp -s f orig || pc_fail "-c changed or removed the input file"
[ -e f.Z ] && pc_fail "-c created f.Z"
[ -s c.out ] || pc_fail "-c wrote nothing to standard output"
compress -d <c.out >back 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "compress -d from standard input: exit status $rc"
cmp -s back orig || pc_fail "decompressed standard output differs from the original"
pc_pass
