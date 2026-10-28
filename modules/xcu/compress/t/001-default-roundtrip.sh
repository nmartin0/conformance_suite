# compress with no options then decompress
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.compress.001
# "compress f" replaces f with f.Z (the lzw suffix) and exits 0; "compress
# -d f.Z" then restores f with the original bytes and removes f.Z.
. "$PC_CORE/pc.sh"
pc_begin XCU.compress.001
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
i=0; while [ "$i" -lt 200 ]; do printf 'line %d of some repetitive test text\n' "$i"; i=$((i + 1)); done >orig || pc_unresolved "setup: cannot write input"
cat orig >f || pc_unresolved "setup: cannot copy input"
pc_test
compress f </dev/null >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "compress: exit status $rc"
[ -f f.Z ] || pc_fail "f.Z was not created"
[ -e f ] && pc_fail "f was not removed"
compress -d f.Z </dev/null >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "compress -d: exit status $rc"
[ -e f.Z ] && pc_fail "f.Z was not removed"
cmp -s f orig || pc_fail "restored file differs from the original"
pc_pass
