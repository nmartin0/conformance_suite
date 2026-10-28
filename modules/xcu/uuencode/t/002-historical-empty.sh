# uuencode of empty input
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uuencode.002
# Empty input without -m gives the begin line, then a line holding only a
# space or a grave accent, then "end".
. "$PC_CORE/pc.sh"
pc_begin XCU.uuencode.002
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
: >in || pc_unresolved "setup: cannot write input"
pc_test
uuencode dest.bin <in >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "exit status $rc"
n=$(wc -l <out); n=$((n + 0))
[ "$n" -eq 3 ] || pc_fail "expected 3 output lines, got $n"
l1=$(sed -n 1p out)
l2=$(sed -n 2p out | tr '\140' ' ')
l3=$(sed -n 3p out)
case $l1 in
begin\ *\ dest.bin) ;;
*) pc_fail "begin line has the wrong shape: $l1" ;;
esac
[ "$l2" = ' ' ] || pc_fail "second line is not a lone space or grave accent"
[ "$l3" = end ] || pc_fail "last line is '$l3', expected 'end'"
pc_pass
