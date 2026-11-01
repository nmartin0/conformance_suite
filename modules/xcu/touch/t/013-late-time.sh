# touch -t accepts a time up to the start of 2038.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# The text makes the range past the Epoch implementation-defined but it
# must reach 0:00:00 on 1 January 2038 UTC, so a failure to set these
# times is a failure of touch.
#
# ASSERTION XCU.touch.013
# MUTANT    touch.late_reject_2030
# MUTANT    touch.late_reject_2037
# MUTANT    touch.late_clamp
# MUTANT    touch.late_boundary_reject
# VARIANT   touch.range_2038
# VARIANT   touch.range_start_2038
. "$PC_CORE/pc.sh"
pc_begin XCU.touch.013
TZ=UTC0; export TZ
nl='
'
same() { [ -e "$1" ] && [ -e "$2" ] && [ ! "$1" -nt "$2" ] && [ ! "$2" -nt "$1" ]; }
# atime_between f lo hi: the access time of f lies strictly between those of lo and hi
atime_between() { [ "$(ls -1ut "$3" "$1" "$2" 2>/dev/null)" = "$3$nl$1$nl$2" ]; }
touch -t 203701011200 r1 || pc_fail "cannot create r1"
touch -t 203712312359.59 r2 || pc_fail "cannot create r2"
touch -t 203801010000 r3 || pc_fail "cannot create r3"
touch -t 203701011159 l1 || pc_fail "cannot create l1"
touch -t 203701011201 h1 || pc_fail "cannot create h1"
for x in f1 f2 f3
do
	: >$x || pc_fail "cannot create $x"
done
pc_test
touch -t 203701011200 f1 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "touch -t 203701011200 exited with status $rc"
same f1 r1 || pc_fail "mid 2037: the time is wrong"
atime_between f1 l1 h1 || pc_fail "mid 2037: the access time is wrong"
touch -t 203712312359.59 f2 || pc_fail "touch -t at the end of 2037 failed"
same f2 r2 || pc_fail "end of 2037: the time is wrong"
touch -t 203801010000 f3 || pc_fail "touch -t at the start of 2038 failed"
same f3 r3 || pc_fail "start of 2038: the time is wrong"
pc_pass
