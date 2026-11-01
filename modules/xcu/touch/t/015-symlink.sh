# touch on a symbolic link operand sets the times of the file the link refers to, and the link remains a link.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.touch.015
# MUTANT    touch.link_replaced
# MUTANT    touch.link_ignored
. "$PC_CORE/pc.sh"
pc_begin XCU.touch.015
TZ=UTC0; export TZ
nl='
'
same() { [ -e "$1" ] && [ -e "$2" ] && [ ! "$1" -nt "$2" ] && [ ! "$2" -nt "$1" ]; }
# atime_between f lo hi: the access time of f lies strictly between those of lo and hi
atime_between() { [ "$(ls -1ut "$3" "$1" "$2" 2>/dev/null)" = "$3$nl$1$nl$2" ]; }
touch -t 200102030405 target || pc_fail "cannot create target"
touch -t 200102030405 ref || pc_fail "cannot create ref"
touch -t 200102030405 t2 || pc_fail "cannot create t2"
ln -s target link || pc_unresolved "cannot make a symbolic link"
ln -s t2 link2 || pc_unresolved "cannot make a symbolic link"
: >ref1 || pc_fail "cannot create ref1"
sleep 1 || pc_fail "sleep failed"
pc_test
touch link link2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "touch exited with status $rc"
sleep 1 || pc_fail "sleep failed"
: >later || pc_fail "cannot create later"
[ -h link ] || pc_fail "the link is no longer a symbolic link"
[ -h link2 ] || pc_fail "the second link is no longer a symbolic link"
for x in target t2
do
	[ -f $x ] && [ ! -h $x ] || pc_fail "$x is not a regular file"
	[ $x -nt ref1 ] && [ later -nt $x ] || pc_fail "$x: the modification time was not set to the current time"
	atime_between $x ref1 later || pc_fail "$x: the access time was not set to the current time"
done
pc_pass
