# uncompress of a gzip-format file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uncompress.007
# "uncompress h.gz" restores h with the original bytes, removes h.gz and
# exits 0.
. "$PC_CORE/pc.sh"
pc_begin XCU.uncompress.007
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
i=0; while [ "$i" -lt 200 ]; do printf 'line %d of some repetitive test text\n' "$i"; i=$((i + 1)); done >orig || pc_unresolved "setup: cannot write input"
cat orig >h || pc_unresolved "setup: cannot copy input"
compress -g h </dev/null >/dev/null 2>&1 || pc_unresolved "setup: compress -g failed"
[ -f h.gz ] || pc_unresolved "setup: compressed file missing"
pc_test
uncompress h.gz </dev/null >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "uncompress h.gz: exit status $rc"
cmp -s h orig || pc_fail "h differs from the original"
[ -e h.gz ] && pc_fail "h.gz was not removed"
pc_pass
