# -depth makes find act on the entries of a directory before the directory; without it a directory comes first.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.find.019
. "$PC_CORE/pc.sh"
pc_begin XCU.find.019
mkdir t t/sub t/empty || pc_fail "cannot create directories"
printf 'a\n' >t/a.txt || pc_fail "cannot create t/a.txt"
printf 'bb\n' >t/B.TXT || pc_fail "cannot create t/B.TXT"
printf 'c\n' >t/sub/c.txt || pc_fail "cannot create t/sub/c.txt"
printf 'd\n' >t/sub/d.c || pc_fail "cannot create t/sub/d.c"
: >t/.hid || pc_fail "cannot create t/.hid"
ln -s a.txt t/l || pc_fail "cannot create symbolic link"
mkfifo t/p || pc_fail "cannot create FIFO"
posof() {
	grep -n -x -F -- "$1" "$2" | cut -d: -f1
}
pc_test
find t -depth >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "find -depth exited with status $rc"
[ "$(posof t out)" = 10 ] || pc_fail "with -depth the operand is not last"
[ "$(posof t/sub out)" -gt "$(posof t/sub/c.txt out)" ] || pc_fail "t/sub does not come after t/sub/c.txt"
[ "$(posof t/sub out)" -gt "$(posof t/sub/d.c out)" ] || pc_fail "t/sub does not come after t/sub/d.c"
find t >out 2>err </dev/null
[ "$(posof t out)" = 1 ] || pc_fail "without -depth the operand is not first"
[ "$(posof t/sub out)" -lt "$(posof t/sub/c.txt out)" ] || pc_fail "without -depth t/sub does not come before t/sub/c.txt"
pc_pass
