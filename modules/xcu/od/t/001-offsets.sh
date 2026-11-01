# od -A d, o and x write the offset of each line's first byte in that base, and the offset after the last byte on a final line, with no blank after it; -A n writes none.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.od.001
# MUTANT    od.Ad_oct
# MUTANT    od.Ao_dec
# MUTANT    od.Ax_dec
# MUTANT    od.An_dec
# MUTANT    od.off_final_missing
# MUTANT    od.off_final_blank
# MUTANT    od.off_final_plus1
# MUTANT    od.off_all_plus1
# MUTANT    od.off_lead_blank
# MUTANT    od.off_glued
# MUTANT    od.off_zero
# VARIANT   od.off_plain
# VARIANT   od.off_wide
# VARIANT   od.gap3
# VARIANT   od.trail_blank
# VARIANT   od.an_empty
# VARIANT   od.block8
# VARIANT   od.block32
# VARIANT   od.hex_upper
. "$PC_CORE/pc.sh"
pc_begin XCU.od.001
LC_ALL=POSIX
export LC_ALL
NL='
'
B=" $(printf '\t')"
awk 'BEGIN { for (i = 0; i < 300; i++) printf "a" }' >f40 || pc_fail "cannot create input"
check_base() {
	base=$1
	od -A "$base" -v -t x1 f40 >out 2>/dev/null </dev/null || return 1
	exp=0
	n=0
	last=
	while IFS= read -r line; do
		# the offset starts at the first character of the line
		case $line in
		[$B]*) return 8 ;;
		esac
		set -- $line
		off=$1
		shift
		case $base in
		d)
			z=${off%%[!0]*}
			off=${off#"$z"}
			[ -z "$off" ] && off=0
			v=$((off))
			;;
		o) v=$((0$off)) ;;
		x) v=$((0x$off)) ;;
		esac
		[ "$v" -eq "$exp" ] || return 3
		exp=$((exp + $#))
		n=$((n + 1))
		last=$line
		lastcount=$#
	done <out
	[ "$exp" -eq 300 ] || return 4
	[ "$n" -ge 2 ] || return 5
	[ "$lastcount" -eq 0 ] || return 6
	case $last in
	*[$B]) return 7 ;;
	esac
	return 0
}
pc_test
for b in d o x; do
	check_base "$b"
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "-A $b: offsets are wrong (check $rc)"
done
od -A n -v -t x1 f40 >out 2>/dev/null </dev/null || pc_fail "-A n failed"
read -r line <out || pc_fail "no output with -A n"
set -- $line
[ "$1" = 61 ] || pc_fail "-A n: the first field is not the first byte"
pc_pass
