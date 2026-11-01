# touch -t sets both times to the given time, interpreting it in the TZ time zone, in the forms CCYYMMDDhhmm, YYMMDDhhmm, MMDDhhmm and with .SS seconds.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Expected times are files made in the setup phase from the full form, in
# the zone with no offset, with the instant worked out by hand; the access
# time is bracketed by files one second either side. A two-digit year of 69
# gives a time before the Epoch, which the text leaves open, so it is not
# used; years beyond 2037 are used only where touch accepts them.
#
# ASSERTION XCU.touch.007
# MUTANT    touch.t_ignores_tz
# MUTANT    touch.t_century_20
# MUTANT    touch.t_century_19
# MUTANT    touch.t_pivot_30
# MUTANT    touch.t_pivot_50
# MUTANT    touch.t_default_year_prev
# MUTANT    touch.t_default_year_epoch
# MUTANT    touch.t_seconds_ignored
# MUTANT    touch.t_sec60_zero
# MUTANT    touch.t_sec60_reject
# MUTANT    touch.t_month_plus1
# MUTANT    touch.t_minutes_zero
# MUTANT    touch.time_mtime_only
# MUTANT    touch.time_atime_only
# MUTANT    touch.only_first_operand
# MUTANT    touch.new_file_current_time
# VARIANT   touch.range_2038
# VARIANT   touch.range_start_2038
. "$PC_CORE/pc.sh"
pc_begin XCU.touch.007
TZ=UTC0; export TZ
nl='
'
same() { [ -e "$1" ] && [ -e "$2" ] && [ ! "$1" -nt "$2" ] && [ ! "$2" -nt "$1" ]; }
# atime_between f lo hi: the access time of f lies strictly between those of lo and hi
atime_between() { [ "$(ls -1ut "$3" "$1" "$2" 2>/dev/null)" = "$3$nl$1$nl$2" ]; }
mk() { touch -t "$2" "$1" || pc_fail "cannot create $1"; }
mk ra 200102030405
mk la 200102030404.59
mk ha 200102030405.01
mk rb 200102030405.30
mk lb 200102030405.29
mk hb 200102030405.31
mk rc 200102030406
mk r99 199902030405
mk r70 197002030405
mk r00 200002030405
mk r37 203702030405
mk rz5 200102030905
mk rz3 200102030105
mk rz3b 200102022205
y=$(date +%Y) || pc_fail "cannot get the year"
mk ry ${y}02030405
r68=no
touch -t 206802030405 r68 2>/dev/null && r68=yes
for x in f1 f2 f3 f4 f5 f6 f7 f8 f9 f10 f11 f12 f13 f14 f15 f16 f17 f18 f19 m1 m2 m3
do
	: >$x || pc_fail "cannot create $x"
done
pc_test
touch -t 200102030405 f1 || pc_fail "touch -t with a four digit year failed"
same f1 ra || pc_fail "CCYYMMDDhhmm: the modification time is wrong"
atime_between f1 la ha || pc_fail "CCYYMMDDhhmm: the access time is wrong"
touch -t 0102030405 f2 || pc_fail "touch -t with a two digit year failed"
same f2 ra || pc_fail "YYMMDDhhmm (01): the time is wrong"
touch -t 9902030405 f3 || pc_fail "touch -t with year 99 failed"
same f3 r99 || pc_fail "YYMMDDhhmm (99): the time is wrong"
touch -t 7002030405 f4 || pc_fail "touch -t with year 70 failed"
same f4 r70 || pc_fail "YYMMDDhhmm (70): the time is wrong"
touch -t 0002030405 f5 || pc_fail "touch -t with year 00 failed"
same f5 r00 || pc_fail "YYMMDDhhmm (00): the time is wrong"
touch -t 3702030405 f6 || pc_fail "touch -t with year 37 failed"
same f6 r37 || pc_fail "YYMMDDhhmm (37): the time is wrong"
if [ $r68 = yes ] && touch -t 6802030405 f7 2>/dev/null; then
	same f7 r68 || pc_fail "YYMMDDhhmm (68): the time is wrong"
fi
touch -t 200102030405.30 f8 || pc_fail "touch -t with seconds failed"
same f8 rb || pc_fail "CCYYMMDDhhmm.SS: the modification time is wrong"
atime_between f8 lb hb || pc_fail "CCYYMMDDhhmm.SS: the access time is wrong"
touch -t 0102030405.30 f9 || pc_fail "touch -t YYMMDDhhmm.SS failed"
same f9 rb || pc_fail "YYMMDDhhmm.SS: the time is wrong"
touch -t 200102030405.60 f10 || pc_fail "touch -t with seconds 60 failed"
same f10 rc || pc_fail "seconds 60 are not one second after 59"
TZ=AAA5 touch -t 200102030405 f11 || pc_fail "touch -t in a zone behind UTC failed"
same f11 rz5 || pc_fail "five hours behind UTC: the time is wrong"
TZ=BBB-3 touch -t 200102030405 f12 || pc_fail "touch -t in a zone ahead of UTC failed"
same f12 rz3 || pc_fail "three hours ahead of UTC: the time is wrong"
TZ=BBB-3 touch -t 200102030105 f13 || pc_fail "touch -t in a zone ahead of UTC failed"
same f13 rz3b || pc_fail "three hours ahead of UTC across midnight: the time is wrong"
touch -t 02030405 f14 || pc_fail "touch -t MMDDhhmm failed"
same f14 ry || pc_fail "MMDDhhmm: the year is not the current one"
y2=$(date +%Y) || pc_fail "cannot get the year"
[ "$y" = "$y2" ] || pc_unresolved "the year changed during the test"
touch -t 200102030405 m1 m2 m3 || pc_fail "touch -t with several operands failed"
for x in m1 m2 m3
do
	same $x ra || pc_fail "operand $x did not get the time"
done
touch -t 200102030405 newf || pc_fail "touch -t on a missing file failed"
[ -f newf ] || pc_fail "touch -t did not create the missing file"
same newf ra || pc_fail "the created file did not get the -t time"
atime_between newf la ha || pc_fail "the created file did not get the -t access time"
pc_pass
