# touch -d accepts seconds of 60 as -t does, giving the time one second after 59.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.touch.016
# MUTANT    touch.d_sec60_reject
# MUTANT    touch.d_sec60_zero
. "$PC_CORE/pc.sh"
pc_begin XCU.touch.016
TZ=UTC0; export TZ
nl='
'
same() { [ -e "$1" ] && [ -e "$2" ] && [ ! "$1" -nt "$2" ] && [ ! "$2" -nt "$1" ]; }
# atime_between f lo hi: the access time of f lies strictly between those of lo and hi
atime_between() { [ "$(ls -1ut "$3" "$1" "$2" 2>/dev/null)" = "$3$nl$1$nl$2" ]; }
touch -t 200102030406 rc || pc_fail "cannot create rc"
touch -t 200102030406.01 r01 || pc_fail "cannot create r01"
: >f1 || pc_fail "cannot create f1"
: >f2 || pc_fail "cannot create f2"
pc_test
touch -d 2001-02-03T04:05:60Z f1 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "touch -d with seconds 60 exited with status $rc"
same f1 rc || pc_fail "-d seconds 60 are not one second after 59"
touch -d "2001-02-03 04:05:60.25" f2 || pc_fail "touch -d with seconds 60 and a fraction failed"
[ -e f2 ] && [ ! rc -nt f2 ] && [ r01 -nt f2 ] || pc_fail "-d seconds 60 with a fraction: wrong time"
pc_pass
