# -ok asks on standard error, runs the utility on an affirmative reply, and is false otherwise.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.find.016
. "$PC_CORE/pc.sh"
pc_begin XCU.find.016
mkdir t t/sub t/empty || pc_fail "cannot create directories"
printf 'a\n' >t/a.txt || pc_fail "cannot create t/a.txt"
printf 'bb\n' >t/B.TXT || pc_fail "cannot create t/B.TXT"
printf 'c\n' >t/sub/c.txt || pc_fail "cannot create t/sub/c.txt"
printf 'd\n' >t/sub/d.c || pc_fail "cannot create t/sub/d.c"
: >t/.hid || pc_fail "cannot create t/.hid"
ln -s a.txt t/l || pc_fail "cannot create symbolic link"
mkfifo t/p || pc_fail "cannot create FIFO"
pc_test
printf 'y\n' | find t -name a.txt \( -ok printf 'ran %s\n' {} \; -o -print \) >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "find -ok (yes) exited with status $rc"
l=$(cat out)
[ "$l" = "ran t/a.txt" ] || pc_fail "after y the output is: $l"
e=$(cat err)
case $e in
*printf*t/a.txt*|*t/a.txt*printf*) ;;
*) pc_fail "the prompt does not name the utility and the file: $e" ;;
esac
e=${e%"${e##*[![:space:]]}"}
case $e in
*'?') ;;
*) pc_fail "the prompt does not end in a question mark: $e" ;;
esac
printf 'n\n' | find t -name a.txt \( -ok printf 'ran %s\n' {} \; -o -print \) >out 2>err
l=$(cat out)
[ "$l" = "t/a.txt" ] || pc_fail "after n the output is: $l"
pc_pass
