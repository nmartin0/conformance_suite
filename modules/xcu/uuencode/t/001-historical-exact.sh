# uuencode historical encoding of a known three-byte input
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uuencode.001
# Without -m, input "Cat" from standard input gives a begin line "begin MODE
# NAME", the single data line "#0V%T", a line holding only a space or a
# grave accent, and the line "end". The decode pathname operand is carried
# unchanged in the begin line.
. "$PC_CORE/pc.sh"
pc_begin XCU.uuencode.001
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'Cat' >in || pc_unresolved "setup: cannot write input"
pc_test
uuencode dest.bin <in >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "exit status $rc"
n=$(wc -l <out); n=$((n + 0))
[ "$n" -eq 4 ] || pc_fail "expected 4 output lines, got $n"
l1=$(sed -n 1p out)
l2=$(sed -n 2p out)
l3=$(sed -n 3p out | tr '\140' ' ')
l4=$(sed -n 4p out)
case $l1 in
begin\ *\ dest.bin) ;;
*) pc_fail "begin line has the wrong shape: $l1" ;;
esac
mode=${l1#begin }
mode=${mode% dest.bin}
case $mode in
'' | *' '*) pc_fail "no single mode field in the begin line: $l1" ;;
esac
[ "$l2" = '#0V%T' ] || pc_fail "data line is '$l2', expected '#0V%T'"
[ "$l3" = ' ' ] || pc_fail "line after the data is not a lone space or grave accent"
[ "$l4" = end ] || pc_fail "last line is '$l4', expected 'end'"
pc_pass
