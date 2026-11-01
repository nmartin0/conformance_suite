# -ok asks on standard error, runs the utility on an affirmative reply, and is false otherwise.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.find.016
# MUTANT   find.ok_prompt_noq
# MUTANT   find.ok_prompt_nopath
# MUTANT   find.ok_prompt_noutil
# MUTANT   find.ok_prompt_stdout
# MUTANT   find.ok_reply_lower
# MUTANT   find.ok_reply_exact
# MUTANT   find.ok_reply_onechar
# MUTANT   find.ok_inverted
# MUTANT   find.ok_always
# MUTANT   find.ok_declined_true
# MUTANT   find.stderr_dropped
# MUTANT   find.stdin_consumed
# VARIANT  find.ok_prompt_default
# VARIANT  find.ok_prompt_nl
# VARIANT  find.ok_prompt_trail
# VARIANT  find.ok_prompt_pathfirst
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
# the reply is a whole line; the locale's affirmative expression in the POSIX locale accepts any line beginning with y or Y
for r in y Y yes Yes yeah; do
	printf '%s\n' "$r" | find t -name a.txt \( -ok printf 'ran %s\n' {} \; -o -print \) >out 2>err
	l=$(cat out)
	[ "$l" = "ran t/a.txt" ] || pc_fail "after '$r' the output is: $l"
done
for r in n N no x '' ' y' 1; do
	printf '%s\n' "$r" | find t -name a.txt \( -ok printf 'ran %s\n' {} \; -o -print \) >out 2>err
	l=$(cat out)
	[ "$l" = "t/a.txt" ] || pc_fail "after '$r' the output is: $l"
done
# two questions take two lines
printf 'Yes\nyes\n' | find t -name '*.txt' \( -ok printf 'ran %s\n' {} \; -o -print \) >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "two -ok questions: find exited with status $rc"
l=$(sort out | tr '\n' ' ')
[ "$l" = "ran t/a.txt ran t/sub/c.txt " ] || pc_fail "after two lines of yes the output is: $l"
# without -ok the standard input is not read
printf 'one\ntwo\n' >in
{ find t -name a.txt -exec true {} \; -print >out 2>err; IFS= read -r l1; } <in
[ "$l1" = one ] || pc_fail "find read from the standard input although there is no -ok: '$l1'"
pc_pass
