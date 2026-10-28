# uncompress -v
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uncompress.006
# With -v uncompress writes a message to standard error for each file and
# nothing to standard output.
. "$PC_CORE/pc.sh"
pc_begin XCU.uncompress.006
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
i=0; while [ "$i" -lt 200 ]; do printf 'line %d of some repetitive test text\n' "$i"; i=$((i + 1)); done >orig || pc_unresolved "setup: cannot write input"
cat orig >f || pc_unresolved "setup: cannot copy input"
compress f </dev/null >/dev/null 2>&1 || pc_unresolved "setup: compress failed"
pc_test
uncompress -v f.Z </dev/null >o 2>e
rc=$?
[ "$rc" -eq 0 ] || pc_fail "exit status $rc"
[ -s e ] || pc_fail "-v wrote nothing to standard error"
[ -s o ] && pc_fail "-v wrote to standard output"
pc_pass
