# With -H, a symbolic link to a directory given as operand is followed even with -l.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.032
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.032
LC_ALL=POSIX; export LC_ALL
mkdir d d/sub || pc_fail "cannot create directories"
: >d/sub/x || pc_fail "cannot create d/sub/x"
ln -s sub d/sl || pc_fail "cannot create symbolic link"
pc_test
ls -H -l d/sl >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls -H -l exited with status $rc: $(cat err)"
l1=$(sed -n 1p out)
case $l1 in
"total "*) ;;
*) pc_fail "first line '$l1' is not a total line, so the directory was not listed" ;;
esac
l2=$(sed -n 2p out)
case $l2 in
-*" x") ;;
*) pc_fail "second line '$l2' does not describe x" ;;
esac
pc_pass
