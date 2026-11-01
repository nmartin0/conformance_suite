# touch -a -m changes both times.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# The options are given separately in either order and grouped.
#
# ASSERTION XCU.touch.006
# MUTANT    touch.am_atime_only
# MUTANT    touch.am_mtime_only
# MUTANT    touch.cur_hour_behind
# MUTANT    touch.cur_hour_ahead
. "$PC_CORE/pc.sh"
pc_begin XCU.touch.006
TZ=UTC0; export TZ
nl='
'
same() { [ -e "$1" ] && [ -e "$2" ] && [ ! "$1" -nt "$2" ] && [ ! "$2" -nt "$1" ]; }
# atime_between f lo hi: the access time of f lies strictly between those of lo and hi
atime_between() { [ "$(ls -1ut "$3" "$1" "$2" 2>/dev/null)" = "$3$nl$1$nl$2" ]; }
# now_between f before after: both times of f lie strictly between those of the two files
now_between() {
	[ -e "$1" ] && [ "$1" -nt "$2" ] && [ "$3" -nt "$1" ] || return 1
	atime_between "$1" "$2" "$3"
}
for x in f1 f2 f3 f4
do
	touch -t 200101021504 $x || pc_fail "cannot create $x"
done
: >ref1 || pc_fail "cannot create ref1"
sleep 1 || pc_fail "sleep failed"
pc_test
touch -a -m f1 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "touch -a -m exited with status $rc"
touch -m -a f2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "touch -m -a exited with status $rc"
touch -am f3 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "touch -am exited with status $rc"
touch -ma f4 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "touch -ma exited with status $rc"
sleep 1 || pc_fail "sleep failed"
: >later || pc_fail "cannot create later"
now_between f1 ref1 later || pc_fail "-a -m: the times are not the current time"
now_between f2 ref1 later || pc_fail "-m -a: the times are not the current time"
now_between f3 ref1 later || pc_fail "-am: the times are not the current time"
now_between f4 ref1 later || pc_fail "-ma: the times are not the current time"
pc_pass
