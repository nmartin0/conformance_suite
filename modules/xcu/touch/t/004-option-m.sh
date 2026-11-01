# touch -m changes the modification time only.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# The untouched access time is compared with two files whose times lie one
# second either side of it; the new modification time is bracketed by files
# made one second before and one second after the run.
#
# ASSERTION XCU.touch.004
# MUTANT    touch.m_also_atime
# MUTANT    touch.m_atime_only
# MUTANT    touch.m_nothing
# MUTANT    touch.m_atime_epoch
# MUTANT    touch.cur_hour_behind
# MUTANT    touch.cur_hour_ahead
. "$PC_CORE/pc.sh"
pc_begin XCU.touch.004
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
touch -t 200101021504.29 lo || pc_fail "cannot create lo"
touch -t 200101021504.31 hi || pc_fail "cannot create hi"
: >ref1 || pc_fail "cannot create ref1"
sleep 1 || pc_fail "sleep failed"
pc_test
touch -m f >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "touch -m exited with status $rc"
sleep 1 || pc_fail "sleep failed"
: >later || pc_fail "cannot create later"
[ -e f ] && [ f -nt ref1 ] && [ later -nt f ] || pc_fail "the modification time was not set to the current time"
atime_between f lo hi || pc_fail "the access time changed"
pc_pass
