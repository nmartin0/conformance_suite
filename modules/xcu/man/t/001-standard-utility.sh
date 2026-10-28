# man of a standard utility
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.man.001
# For the name of a standard utility, man exits 0 and writes a non-empty
# description to standard output, and with two such names it still succeeds
# and writes at least as much.
. "$PC_CORE/pc.sh"
pc_begin XCU.man.001
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
PAGER=cat; export PAGER
pc_test
man ls </dev/null >out 2>err || pc_fail "man ls: nonzero status"
[ -s out ] || pc_fail "man ls wrote nothing to standard output"
man cat </dev/null >out2 2>err || pc_fail "man cat: nonzero status"
[ -s out2 ] || pc_fail "man cat wrote nothing"
man ls cat </dev/null >both 2>err || pc_fail "man ls cat: nonzero status"
s1=$(wc -c <out | tr -d ' '); s2=$(wc -c <both | tr -d ' ')
[ "$s2" -ge "$s1" ] || pc_fail "two names gave less output than one"
pc_pass
