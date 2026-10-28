# In the long format a symbolic link is shown as name, space, arrow, space, link contents.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.012
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.012
LC_ALL=POSIX; export LC_ALL
mkdir d || pc_fail "cannot create d"
: >d/fl || pc_fail "cannot create d/fl"
ln -s fl d/lk || pc_fail "cannot create symbolic link"
pc_test
l=$(ls -ld d/lk) || pc_fail "ls -ld d/lk failed"
case $l in
l*" d/lk -> fl") ;;
*) pc_fail "line is: $l" ;;
esac
ls -l d >out 2>err </dev/null
found=no
while IFS= read -r l; do
	case $l in
	l*" lk -> fl") found=yes ;;
	esac
done <out
[ "$found" = yes ] || pc_fail "ls -l d does not show 'lk -> fl' for the link"
pc_pass
