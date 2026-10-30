# A line whose offset starts with > is applied only after the preceding line matched, and both messages are written.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.file.011
. "$PC_CORE/pc.sh"
pc_begin XCU.file.011
printf '0\tstring\tAB\tfirst\n>2\tstring\tC\tthird\n' >magic || pc_fail "cannot create magic"
printf 'ABC' >f1 || pc_fail "cannot create f1"
printf 'ABD' >f2 || pc_fail "cannot create f2"
printf 'XBC' >f3 || pc_fail "cannot create f3"
pc_test
file -m magic f1 >out 2>err </dev/null
l=$(cat out)
case $l in
*first*third*) ;;
*) pc_fail "ABC: $l $(cat err)" ;;
esac
file -m magic f2 >out 2>err </dev/null
l=$(cat out)
case $l in
*first*) ;;
*) pc_fail "ABD lacks the first message: $l" ;;
esac
case $l in
*third*) pc_fail "ABD got the continuation message: $l" ;;
esac
file -m magic f3 >out 2>err </dev/null
l=$(cat out)
case $l in
*first*|*third*) pc_fail "XBC matched: $l" ;;
esac
pc_pass
