# Without options touch sets both the modification and the access time of an existing file to the current time.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Time is bracketed by two files made one second before and one second after
# the run, so the check does not depend on the clock in any other way.
#
# ASSERTION XCU.touch.003
# MUTANT    touch.cur_hour_ahead
# MUTANT    touch.cur_hour_behind
# MUTANT    touch.cur_fixed
# MUTANT    touch.cur_mtime_only
# MUTANT    touch.cur_atime_only
# MUTANT    touch.cur_nothing
# MUTANT    touch.only_first_operand
. "$PC_CORE/pc.sh"
pc_begin XCU.touch.003
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
touch -t 200101021504 f || pc_fail "cannot create f"
touch -t 200101021504 g || pc_fail "cannot create g"
: >ref1 || pc_fail "cannot create ref1"
sleep 1 || pc_fail "sleep failed"
pc_test
touch f g >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "touch exited with status $rc"
sleep 1 || pc_fail "sleep failed"
: >later || pc_fail "cannot create later"
now_between f ref1 later || pc_fail "f: the times are not the current time"
now_between g ref1 later || pc_fail "g: the times are not the current time"
[ ! -s out ] || pc_fail "touch wrote to standard output"
pc_pass
