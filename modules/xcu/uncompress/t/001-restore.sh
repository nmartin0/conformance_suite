# uncompress restores the file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uncompress.001
# "uncompress f.Z" restores f with the original bytes, removes f.Z and exits
# 0.
. "$PC_CORE/pc.sh"
pc_begin XCU.uncompress.001
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
i=0; while [ "$i" -lt 200 ]; do printf 'line %d of some repetitive test text\n' "$i"; i=$((i + 1)); done >orig || pc_unresolved "setup: cannot write input"
cat orig >f || pc_unresolved "setup: cannot copy input"
compress f </dev/null >/dev/null 2>&1 || pc_unresolved "setup: compress failed"
[ -f f.Z ] || pc_unresolved "setup: compressed file missing"
pc_test
uncompress f.Z </dev/null >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "uncompress f.Z: exit status $rc"
cmp -s f orig || pc_fail "f differs from the original"
[ -e f.Z ] && pc_fail "f.Z was not removed"
pc_pass
