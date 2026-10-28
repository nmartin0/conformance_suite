# When one file is a prefix of the other, cmp exits 1 and writes an EOF diagnostic naming the shorter file to standard error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cmp.005
. "$PC_CORE/pc.sh"
pc_begin XCU.cmp.005
printf 'abc' >f1 || pc_fail "cannot create f1"
printf 'abcd' >f2 || pc_fail "cannot create f2"
pc_test
cmp f1 f2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "cmp exited with status $rc, expected 1"
e=$(cat err)
case $e in
"cmp: EOF on f1"|"cmp: EOF on f1 "*) ;;
*) pc_fail "diagnostic is: $e" ;;
esac
cmp f2 f1 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "cmp (reversed) exited with status $rc, expected 1"
e=$(cat err)
case $e in
"cmp: EOF on f1"|"cmp: EOF on f1 "*) ;;
*) pc_fail "reversed diagnostic is: $e" ;;
esac
pc_pass
