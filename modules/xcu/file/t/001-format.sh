# file writes one line per operand in the form name, colon, space, a type description.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.file.001
. "$PC_CORE/pc.sh"
pc_begin XCU.file.001
printf 'hello\n' >f1 || pc_fail "cannot create f1"
printf 'world\n' >f2 || pc_fail "cannot create f2"
pc_test
file f1 f2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "file exited with status $rc"
n=$(sed -n '$=' out)
[ "$n" = 2 ] || pc_fail "file wrote $n lines, expected 2"
l1=$(sed -n 1p out)
l2=$(sed -n 2p out)
case $l1 in
"f1: "?*) ;;
*) pc_fail "first line is: $l1" ;;
esac
case $l2 in
"f2: "?*) ;;
*) pc_fail "second line is: $l2" ;;
esac
pc_pass
