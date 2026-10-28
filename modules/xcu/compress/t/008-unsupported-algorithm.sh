# compress with an algorithm name no system supports
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.compress.008
# "compress -m" with an algorithm name that is certainly unsupported exits
# with a status greater than 2 and leaves the file unchanged and in place.
. "$PC_CORE/pc.sh"
pc_begin XCU.compress.008
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
i=0; while [ "$i" -lt 200 ]; do printf 'line %d of some repetitive test text\n' "$i"; i=$((i + 1)); done >orig || pc_unresolved "setup: cannot write input"
cat orig >f || pc_unresolved "setup: cannot copy input"
pc_test
compress -m zq-no-such-algorithm-7731 f </dev/null >out 2>err
rc=$?
[ "$rc" -gt 2 ] || pc_fail "unsupported algorithm: exit status $rc, expected greater than 2"
[ -f f ] || pc_fail "the input file is gone"
cmp -s f orig || pc_fail "the input file was changed"
pc_pass
