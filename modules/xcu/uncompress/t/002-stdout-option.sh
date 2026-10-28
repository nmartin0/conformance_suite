# uncompress -c
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uncompress.002
# "uncompress -c f.Z" writes the original bytes to standard output and
# leaves f.Z present and unchanged, with no f created.
. "$PC_CORE/pc.sh"
pc_begin XCU.uncompress.002
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
i=0; while [ "$i" -lt 200 ]; do printf 'line %d of some repetitive test text\n' "$i"; i=$((i + 1)); done >orig || pc_unresolved "setup: cannot write input"
cat orig >f || pc_unresolved "setup: cannot copy input"
compress f </dev/null >/dev/null 2>&1 || pc_unresolved "setup: compress failed"
cat f.Z >keep.Z || pc_unresolved "setup: cannot copy compressed file"
pc_test
uncompress -c f.Z </dev/null >o 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "uncompress -c: exit status $rc"
cmp -s o orig || pc_fail "standard output differs from the original"
[ -f f.Z ] || pc_fail "-c removed f.Z"
cmp -s f.Z keep.Z || pc_fail "-c changed f.Z"
[ -e f ] && pc_fail "-c created f"
pc_pass
