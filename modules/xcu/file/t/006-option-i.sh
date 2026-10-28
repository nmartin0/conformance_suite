# With -i a regular file is described as regular file without further classification, empty or not.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.file.006
. "$PC_CORE/pc.sh"
pc_begin XCU.file.006
printf 'hello\n' >f || pc_fail "cannot create f"
: >e || pc_fail "cannot create e"
pc_test
file -i f e >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "file -i exited with status $rc"
l1=$(sed -n 1p out)
l2=$(sed -n 2p out)
case $l1 in
"f: "*"regular file"*) ;;
*) pc_fail "first line is: $l1" ;;
esac
case $l2 in
"e: "*"regular file"*) ;;
*) pc_fail "second line is: $l2" ;;
esac
pc_pass
