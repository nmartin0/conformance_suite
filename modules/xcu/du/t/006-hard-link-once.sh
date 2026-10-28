# A file that occurs under several names is written only once.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.du.006
. "$PC_CORE/pc.sh"
pc_begin XCU.du.006
mkdir d || pc_fail "cannot create d"
printf 'abc\n' >d/a || pc_fail "cannot create d/a"
ln d/a d/b || pc_fail "cannot create hard link"
pc_test
du -a d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "du -a exited with status $rc"
count=0
while read -r sz name; do
	case $name in
	d/a|d/b) count=$((count + 1)) ;;
	esac
done <out
[ "$count" -eq 1 ] || pc_fail "the linked file was written $count times"
pc_pass
