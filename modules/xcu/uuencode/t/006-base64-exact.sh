# uuencode -m known input
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uuencode.006
# With -m, input "Cat" gives the begin line "begin-base64 MODE NAME", the
# single line "Q2F0" and the line "====".
. "$PC_CORE/pc.sh"
pc_begin XCU.uuencode.006
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
:
pc_test
printf 'Cat' | uuencode -m dest.bin >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "exit status $rc"
n=$(wc -l <out); n=$((n + 0))
[ "$n" -eq 3 ] || pc_fail "expected 3 output lines, got $n"
l1=$(sed -n 1p out)
l2=$(sed -n 2p out)
l3=$(sed -n 3p out)
case $l1 in
begin-base64\ *\ dest.bin) ;;
*) pc_fail "begin line has the wrong shape: $l1" ;;
esac
mode=${l1#begin-base64 }
mode=${mode% dest.bin}
case $mode in
'' | *' '*) pc_fail "no single mode field in the begin line: $l1" ;;
esac
[ "$l2" = Q2F0 ] || pc_fail "data line is '$l2', expected 'Q2F0'"
[ "$l3" = '====' ] || pc_fail "last line is '$l3', expected '===='"
pc_pass
