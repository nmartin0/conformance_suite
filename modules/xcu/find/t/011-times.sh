# -mtime, -atime and -ctime compare the age in whole days: 0 is less than a day, +n more than n days, -n fewer.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.find.011
# MUTANT   find.time_plus_ge
# MUTANT   find.time_minus_le
# MUTANT   find.time_exact_minus1
# MUTANT   find.time_exact_plus1
# MUTANT   find.time_sign_ignored
# MUTANT   find.time_atime_as_mtime
# MUTANT   find.time_mtime_as_atime
# MUTANT   find.time_ctime_as_mtime
# MUTANT   find.time_mtime_as_ctime
# MUTANT   find.time_atime_as_ctime
. "$PC_CORE/pc.sh"
pc_begin XCU.find.011
same_set() {
	s_a=$(printf '%s\n' $1 | sort)
	s_b=$(printf '%s\n' $2 | sort)
	[ "$s_a" = "$s_b" ]
}

# ago SECONDS: the CCYYMMDDhhmm form, in UTC, of that long before now
ago() {
	a_t=$(($(date +%s) - $1))
	a_d=$((a_t / 86400))
	a_s=$((a_t % 86400))
	a_z=$((a_d + 719468))
	a_e=$((a_z / 146097))
	a_o=$((a_z - a_e * 146097))
	a_y=$(((a_o - a_o / 1460 + a_o / 36524 - a_o / 146096) / 365))
	a_doy=$((a_o - (365 * a_y + a_y / 4 - a_y / 100)))
	a_mp=$(((5 * a_doy + 2) / 153))
	a_day=$((a_doy - (153 * a_mp + 2) / 5 + 1))
	if [ "$a_mp" -lt 10 ]; then a_m=$((a_mp + 3)); else a_m=$((a_mp - 9)); fi
	a_y=$((a_y + a_e * 400))
	[ "$a_m" -le 2 ] && a_y=$((a_y + 1))
	printf '%04d%02d%02d%02d%02d' "$a_y" "$a_m" "$a_day" $((a_s / 3600)) $(((a_s % 3600) / 60))
}
mkdir tm || pc_unresolved "cannot create tm"
# ages of two and a half and nine and a half days: whole days 2 and 9, far from a boundary
t2=$(ago $((2 * 86400 + 43200))) || pc_unresolved "cannot work out a time"
t9=$(ago $((9 * 86400 + 43200))) || pc_unresolved "cannot work out a time"
TZ=UTC0 touch -t "$t2" tm/d2 || pc_unresolved "cannot set the time of tm/d2"
TZ=UTC0 touch -t "$t9" tm/d9 || pc_unresolved "cannot set the time of tm/d9"
: >tm/fresh || pc_unresolved "cannot create tm/fresh"
# modified long ago but accessed now, and the reverse
TZ=UTC0 touch -t "$t9" tm/mixa || pc_unresolved "cannot set the time of tm/mixa"
touch -a tm/mixa || pc_unresolved "cannot set the access time of tm/mixa"
TZ=UTC0 touch -t "$t9" tm/mixm || pc_unresolved "cannot set the time of tm/mixm"
touch -m tm/mixm || pc_unresolved "cannot set the modification time of tm/mixm"
pc_test
chk() {
	got=$(find tm -type f "$1" "$2" 2>err </dev/null)
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "$1 $2: find exited with status $rc"
	same_set "$got" "$3" || pc_fail "$1 $2: found '$got', expected '$3'"
}
chk -mtime 0 "tm/fresh tm/mixm"
chk -mtime 2 "tm/d2"
chk -mtime 9 "tm/d9 tm/mixa"
chk -mtime +2 "tm/d9 tm/mixa"
chk -mtime +1 "tm/d2 tm/d9 tm/mixa"
chk -mtime +8 "tm/d9 tm/mixa"
chk -mtime -2 "tm/fresh tm/mixm"
chk -mtime -3 "tm/fresh tm/d2 tm/mixm"
chk -mtime -10 "tm/fresh tm/d2 tm/d9 tm/mixa tm/mixm"
chk -atime 0 "tm/fresh tm/mixa"
chk -atime 2 "tm/d2"
chk -atime 9 "tm/d9 tm/mixm"
chk -atime +2 "tm/d9 tm/mixm"
chk -atime -3 "tm/fresh tm/d2 tm/mixa"
chk -ctime 0 "tm/fresh tm/d2 tm/d9 tm/mixa tm/mixm"
chk -ctime +365 ""
chk -ctime 2 ""
pc_pass
