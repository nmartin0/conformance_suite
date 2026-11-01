# touch -d accepts YYYY-MM-DDThh:mm:SS with an optional fraction and Z, takes T or a space, and reads a time without Z in the TZ time zone.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# A fraction need not be kept in full: the file system may hold coarser
# times, down to whole seconds, and the stored value is the greatest it
# supports that is not above the one asked for. So a time with a fraction
# must land from its whole second up to, but not including, the next one.
#
# ASSERTION XCU.touch.008
# MUTANT    touch.d_ignores_tz
# MUTANT    touch.d_z_as_local
# MUTANT    touch.d_space_rejected
# MUTANT    touch.d_T_rejected
# MUTANT    touch.d_frac_rejected
# MUTANT    touch.d_comma_rejected
# MUTANT    touch.d_frac_long_rejected
# MUTANT    touch.d_frac_as_seconds
# MUTANT    touch.d_frac_rounds
# MUTANT    touch.d_seconds_ignored
# MUTANT    touch.time_mtime_only
# MUTANT    touch.time_atime_only
# MUTANT    touch.new_file_current_time
# MUTANT    touch.only_first_operand
# VARIANT   touch.frac_floor
# VARIANT   touch.frac_ms
. "$PC_CORE/pc.sh"
pc_begin XCU.touch.008
TZ=UTC0; export TZ
nl='
'
same() { [ -e "$1" ] && [ -e "$2" ] && [ ! "$1" -nt "$2" ] && [ ! "$2" -nt "$1" ]; }
# atime_between f lo hi: the access time of f lies strictly between those of lo and hi
atime_between() { [ "$(ls -1ut "$3" "$1" "$2" 2>/dev/null)" = "$3$nl$1$nl$2" ]; }
# whole_second f lo hi: the modification time of f is not before lo and before hi
whole_second() { [ -e "$1" ] && [ ! "$2" -nt "$1" ] && [ "$3" -nt "$1" ]; }
mk() { touch -t "$2" "$1" || pc_fail "cannot create $1"; }
mk ra 200102030405
mk la 200102030404.59
mk ha 200102030405.01
mk r06 200102030405.06
mk r07 200102030405.07
mk rz5 200102030905
n=0
for x in f1 f2 f3 f4 f5 f6 f7 f8 f9 f10 f11 f12 f13 f14 f15 f16 f17 f18 m1 m2
do
	: >$x || pc_fail "cannot create $x"
done
pc_test
touch -d 2001-02-03T04:05:00Z f1 || pc_fail "touch -d with Z failed"
same f1 ra || pc_fail "-d ...Z: the modification time is wrong"
atime_between f1 la ha || pc_fail "-d ...Z: the access time is wrong"
touch -d "2001-02-03 04:05:00Z" f2 || pc_fail "touch -d with a space and Z failed"
same f2 ra || pc_fail "-d with a space: the time is wrong"
touch -d 2001-02-03T04:05:00 f3 || pc_fail "touch -d without Z failed"
same f3 ra || pc_fail "-d without Z in a zone with no offset: the time is wrong"
touch -d "2001-02-03 04:05:00" f4 || pc_fail "touch -d with a space and no Z failed"
same f4 ra || pc_fail "-d with a space and no Z: the time is wrong"
TZ=AAA5 touch -d 2001-02-03T04:05:00 f5 || pc_fail "touch -d without Z in another zone failed"
same f5 rz5 || pc_fail "-d without Z is not read in the TZ zone"
TZ=AAA5 touch -d "2001-02-03 04:05:00" f6 || pc_fail "touch -d with a space in another zone failed"
same f6 rz5 || pc_fail "-d with a space and no Z is not read in the TZ zone"
TZ=AAA5 touch -d 2001-02-03T04:05:00Z f7 || pc_fail "touch -d with Z in another zone failed"
same f7 ra || pc_fail "-d with Z is affected by the TZ zone"
touch -d 2001-02-03T04:05:06Z f8 || pc_fail "touch -d with seconds failed"
whole_second f8 r06 r07 || pc_fail "-d seconds: the time is wrong"
touch -d 2001-02-03T04:05:06.25Z f9 || pc_fail "touch -d with a fraction failed"
whole_second f9 r06 r07 || pc_fail "-d with .25: the time is not within the second"
touch -d 2001-02-03T04:05:06,25Z f10 || pc_fail "touch -d with a comma fraction failed"
whole_second f10 r06 r07 || pc_fail "-d with ,25: the time is not within the second"
touch -d 2001-02-03T04:05:06.75Z f11 || pc_fail "touch -d with .75 failed"
whole_second f11 r06 r07 || pc_fail "-d with .75: the time is not within the second"
touch -d 2001-02-03T04:05:06.250000000000Z f12 || pc_fail "touch -d with a long fraction failed"
whole_second f12 r06 r07 || pc_fail "-d with a long fraction: the time is not within the second"
touch -d 2001-02-03T04:05:06.999999999999Z f13 || pc_fail "touch -d with a long fraction of nines failed"
whole_second f13 r06 r07 || pc_fail "-d with a fraction of nines: the time is not within the second"
touch -d 2001-02-03T04:05:06.25 f14 || pc_fail "touch -d with a fraction and no Z failed"
whole_second f14 r06 r07 || pc_fail "-d with a fraction and no Z: the time is wrong"
touch -d "2001-02-03 04:05:06,75Z" f15 || pc_fail "touch -d with a space and a fraction failed"
whole_second f15 r06 r07 || pc_fail "-d with a space and a fraction: the time is wrong"
touch -d 2001-02-03T04:05:00Z newf || pc_fail "touch -d on a missing file failed"
[ -f newf ] || pc_fail "touch -d did not create the missing file"
same newf ra || pc_fail "the created file did not get the -d time"
atime_between newf la ha || pc_fail "the created file did not get the -d access time"
touch -d 2001-02-03T04:05:00Z m1 m2 || pc_fail "touch -d with several operands failed"
same m1 ra && same m2 ra || pc_fail "not every operand got the -d time"
touch -t 200102030405.06 f17 || pc_fail "touch -t failed"
touch -d 2001-02-03T04:05:06Z f18 || pc_fail "touch -d failed"
same f17 f18 || pc_fail "-t and -d give different times for the same instant"
pc_pass
