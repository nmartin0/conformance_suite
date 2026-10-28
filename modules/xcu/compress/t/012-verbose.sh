# compress -v writes to standard error
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.compress.012
# With -v compress writes something to standard error for a file it
# compresses, and nothing to standard output; without -v it writes nothing
# to standard error.
. "$PC_CORE/pc.sh"
pc_begin XCU.compress.012
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
i=0; while [ "$i" -lt 200 ]; do printf 'line %d of some repetitive test text\n' "$i"; i=$((i + 1)); done >orig || pc_unresolved "setup: cannot write input"
cat orig >f && cat orig >g || pc_unresolved "setup: cannot copy input"
pc_test
compress -v f </dev/null >o 2>e
rc=$?
[ "$rc" -eq 0 ] || pc_fail "compress -v: exit status $rc"
[ -s e ] || pc_fail "-v wrote nothing to standard error"
[ -s o ] && pc_fail "-v wrote to standard output"
compress g </dev/null >o2 2>e2
[ -s e2 ] && pc_fail "without -v something was written to standard error"
pc_pass
