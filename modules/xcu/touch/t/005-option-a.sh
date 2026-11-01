# touch -a changes the access time only.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.touch.005
# MUTANT    touch.a_also_mtime
# MUTANT    touch.a_mtime_only
# MUTANT    touch.a_nothing
# MUTANT    touch.a_mtime_epoch
# MUTANT    touch.cur_hour_behind
# MUTANT    touch.cur_hour_ahead
. "$PC_CORE/pc.sh"
pc_begin XCU.touch.005
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
touch -t 200101021504.30 f || pc_fail "cannot create f"
touch -t 200101021504.30 f0 || pc_fail "cannot create f0"
: >ref1 || pc_fail "cannot create ref1"
sleep 1 || pc_fail "sleep failed"
pc_test
touch -a f >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "touch -a exited with status $rc"
sleep 1 || pc_fail "sleep failed"
: >later || pc_fail "cannot create later"
atime_between f ref1 later || pc_fail "the access time was not set to the current time"
same f f0 || pc_fail "the modification time changed"
pc_pass
