# With -m, the tests of the named file are applied: a matching string at an offset prints the message, a non-matching file does not.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.file.009
. "$PC_CORE/pc.sh"
pc_begin XCU.file.009
tab=$(printf '\t')
printf '0\tstring\tPCMAGIC\tpcmagic file\n' >magic || pc_fail "cannot create magic"
printf 'PCMAGIC data\n' >f1 || pc_fail "cannot create f1"
printf 'other data\n' >f2 || pc_fail "cannot create f2"
pc_test
file -m magic f1 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "file -m exited with status $rc"
l=$(cat out)
case $l in
"f1: "*"pcmagic file"*) ;;
*) pc_fail "matching file: $l" ;;
esac
file -m magic f2 >out 2>err </dev/null
l=$(cat out)
case $l in
*"pcmagic file"*) pc_fail "non-matching file was described by the test: $l" ;;
esac
file -M magic f1 >out 2>err </dev/null
l=$(cat out)
case $l in
"f1: "*"pcmagic file"*) ;;
*) pc_fail "-M: matching file: $l" ;;
esac
pc_pass
