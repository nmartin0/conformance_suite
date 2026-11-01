# With a time given by -t, -d or -r, -m changes only the modification time and -a only the access time.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Every file starts with both times at 03:00:30 on 1 January 2003; the
# other time must stay there, checked against files one second either side
# (access time) and a file with the same time (modification time).
#
# ASSERTION XCU.touch.014
# MUTANT    touch.ignore_am_with_time
# MUTANT    touch.other_time_now
# MUTANT    touch.swap_am_with_time
. "$PC_CORE/pc.sh"
pc_begin XCU.touch.014
TZ=UTC0; export TZ
nl='
'
same() { [ -e "$1" ] && [ -e "$2" ] && [ ! "$1" -nt "$2" ] && [ ! "$2" -nt "$1" ]; }
# atime_between f lo hi: the access time of f lies strictly between those of lo and hi
atime_between() { [ "$(ls -1ut "$3" "$1" "$2" 2>/dev/null)" = "$3$nl$1$nl$2" ]; }
mk() { touch -t "$2" "$1" || pc_fail "cannot create $1"; }
mk keep 200301010300.30
mk klo 200301010300.29
mk khi 200301010300.31
mk ra 200102030405
mk la 200102030404.59
mk ha 200102030405.01
touch -t 200101021504.30 ref || pc_fail "cannot create ref"
touch -a -t 200205060708.40 ref || pc_fail "cannot set the access time of ref"
mk rm 200101021504.30
mk lo 200205060708.39
mk hi 200205060708.41
for x in t1 t2 d1 d2 r1 r2
do
	mk $x 200301010300.30
done
pc_test
touch -m -t 200102030405 t1 || pc_fail "touch -m -t failed"
same t1 ra || pc_fail "-m -t: the modification time is wrong"
atime_between t1 klo khi || pc_fail "-m -t: the access time changed"
touch -a -t 200102030405 t2 || pc_fail "touch -a -t failed"
atime_between t2 la ha || pc_fail "-a -t: the access time is wrong"
same t2 keep || pc_fail "-a -t: the modification time changed"
touch -m -d 2001-02-03T04:05:00Z d1 || pc_fail "touch -m -d failed"
same d1 ra || pc_fail "-m -d: the modification time is wrong"
atime_between d1 klo khi || pc_fail "-m -d: the access time changed"
touch -a -d 2001-02-03T04:05:00Z d2 || pc_fail "touch -a -d failed"
atime_between d2 la ha || pc_fail "-a -d: the access time is wrong"
same d2 keep || pc_fail "-a -d: the modification time changed"
touch -m -r ref r1 || pc_fail "touch -m -r failed"
same r1 rm || pc_fail "-m -r: the modification time is wrong"
atime_between r1 klo khi || pc_fail "-m -r: the access time changed"
touch -a -r ref r2 || pc_fail "touch -a -r failed"
atime_between r2 lo hi || pc_fail "-a -r: the access time is wrong"
same r2 keep || pc_fail "-a -r: the modification time changed"
pc_pass
