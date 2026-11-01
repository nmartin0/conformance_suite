# In the POSIX locale the long format date is month, day and hour:minute for a recent file and month, day and year for an old or future one.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.028
# MUTANT    ls.d_old_time
# MUTANT    ls.d_future_time
# MUTANT    ls.d_recent_year
# MUTANT    ls.d_day_padded
# MUTANT    ls.d_month_number
# MUTANT    ls.d_seconds
# MUTANT    ls.d_window_short
# MUTANT    ls.d_window_long
# VARIANT   ls.acl_flag
# VARIANT   ls.names_numeric
#
# The year of a future file rests on touch -t accepting a date in 2037,
# which the touch page requires.
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.028
LC_ALL=POSIX; export LC_ALL
TZ=UTC0; export TZ
mkdir d || pc_fail "cannot create d"
touch -t 200101021504 d/old || pc_fail "cannot set time"
touch -t 203701021504 d/fut || pc_fail "cannot set time"
: >d/new || pc_fail "cannot create d/new"
# Files 60 and 300 days old, made with the calendar arithmetic of the shell.
# days_from_civil Y M D: number of days since 1970-01-01 in dfc
days_from_civil() {
	dy=$1; dm=${2#0}; dd=${3#0}
	[ "$dm" -le 2 ] && dy=$((dy - 1))
	dera=$((dy / 400))
	dyoe=$((dy - dera * 400))
	dmp=$(((dm + 9) % 12))
	ddoy=$(((153 * dmp + 2) / 5 + dd - 1))
	ddoe=$((dyoe * 365 + dyoe / 4 - dyoe / 100 + ddoy))
	dfc=$((dera * 146097 + ddoe - 719468))
}
# civil_from_days N: year, month and day of the day number in cy cm cd
civil_from_days() {
	cz=$(($1 + 719468))
	cera=$((cz / 146097))
	cdoe=$((cz - cera * 146097))
	cyoe=$(((cdoe - cdoe / 1460 + cdoe / 36524 - cdoe / 146096) / 365))
	cy=$((cyoe + cera * 400))
	cdoy=$((cdoe - (365 * cyoe + cyoe / 4 - cyoe / 100)))
	cmp=$(((5 * cdoy + 2) / 153))
	cd=$((cdoy - (153 * cmp + 2) / 5 + 1))
	if [ "$cmp" -lt 10 ]; then cm=$((cmp + 3)); else cm=$((cmp - 9)); fi
	[ "$cm" -le 2 ] && cy=$((cy + 1))
	return 0
}
today=$(date +%Y%m%d) || pc_unresolved "date failed"
days_from_civil "${today%????}" "$(echo "$today" | cut -c5-6)" "$(echo "$today" | cut -c7-8)"
now=$dfc
civil_from_days $((now - 60))
y60=$cy; m60=$cm; d60=$cd
civil_from_days $((now - 300))
y300=$cy; m300=$cm; d300=$cd
touch -t "$(printf '%04d%02d%02d1200' "$y60" "$m60" "$d60")" d/mid60 || pc_unresolved "cannot set time"
touch -t "$(printf '%04d%02d%02d1200' "$y300" "$m300" "$d300")" d/mid300 || pc_unresolved "cannot set time"
set -- Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
shift $((m60 - 1)); n60=$1
set -- Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
shift $((m300 - 1)); n300=$1
pc_test
set -- $(ls -l d/old)
[ "$#" -eq 9 ] || pc_unresolved "unexpected number of fields ($#)"
[ "$6 $7 $8" = "Jan 2 2001" ] || pc_fail "old file date is '$6 $7 $8'"
set -- $(ls -l d/fut)
[ "$#" -eq 9 ] || pc_unresolved "unexpected number of fields ($#)"
[ "$6 $7 $8" = "Jan 2 2037" ] || pc_fail "future file date is '$6 $7 $8'"
set -- $(ls -l d/new)
[ "$#" -eq 9 ] || pc_unresolved "unexpected number of fields ($#)"
case $8 in
[0-9][0-9]:[0-9][0-9]) ;;
*) pc_fail "recent file time is '$8'" ;;
esac
# modified 60 days ago: within the last six months, time of day; 300 days
# ago: more than six months, year
set -- $(ls -l d/mid60)
[ "$#" -eq 9 ] || pc_unresolved "unexpected number of fields ($#)"
[ "$6 $7 $8" = "$n60 $d60 12:00" ] || pc_fail "file modified 60 days ago: '$6 $7 $8', expected '$n60 $d60 12:00'"
set -- $(ls -l d/mid300)
[ "$#" -eq 9 ] || pc_unresolved "unexpected number of fields ($#)"
[ "$6 $7 $8" = "$n300 $d300 $y300" ] || pc_fail "file modified 300 days ago: '$6 $7 $8', expected '$n300 $d300 $y300'"
pc_pass
