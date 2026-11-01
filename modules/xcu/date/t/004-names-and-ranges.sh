# English names and numeric ranges of the conversions in the POSIX locale.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.date.004
# MUTANT    date.a_full
# MUTANT    date.A_abbr
# MUTANT    date.b_full
# MUTANT    date.B_abbr
# MUTANT    date.a_upper
# MUTANT    date.A_lower
# MUTANT    date.b_upper
# MUTANT    date.B_lower
# MUTANT    date.a_shift
# MUTANT    date.A_shift
# MUTANT    date.b_shift
# MUTANT    date.B_shift
# MUTANT    date.p_lower
# MUTANT    date.p_swapped
# MUTANT    date.w_shift
# MUTANT    date.u_plus1
# MUTANT    date.I_off
# MUTANT    date.H_24
# MUTANT    date.I_zero
# MUTANT    date.M_60
# MUTANT    date.S_61
# MUTANT    date.d_zero
# MUTANT    date.m_13
# MUTANT    date.j_zero
# MUTANT    date.w_7
# MUTANT    date.u_zero
# MUTANT    date.U_54
# MUTANT    date.W_54
# MUTANT    date.V_zero
# MUTANT    date.y_100
# MUTANT    date.e_zero
# VARIANT   date.leap_second
. "$PC_CORE/pc.sh"
pc_begin XCU.date.004
LC_ALL=C; export LC_ALL
TZ=UTC0; export TZ
# dec STR: v = STR as a decimal number without leading zeros.
dec() {
	v=$1
	while :; do
		case $v in
		0?*) v=${v#0} ;;
		*) break ;;
		esac
	done
}
inrange() { # inrange NAME VALUE LOW HIGH
	case $2 in
	'' | *[!0-9]*) pc_fail "$1 is '$2', not a number" ;;
	esac
	dec "$2"
	[ "$v" -ge "$3" ] && [ "$v" -le "$4" ] || pc_fail "$1 is '$2', outside $3-$4"
}
pc_test
out=$(date '+%a|%A|%b|%B|%p|%j|%u|%w|%U|%W|%V|%H|%I|%M|%S|%e|%d|%y|%m')
set -f
IFS='|'
set -- $out
IFS=' 	
'
set +f
[ "$#" -eq 19 ] || pc_fail "expected 19 fields, got $# from '$out'"
a=$1 A=$2 b=$3 B=$4 p=$5 j=$6 u=$7 w=$8 U=$9
shift 9
W=$1 V=$2 H=$3 I=$4 M=$5 S=$6 e=$7 d=$8 y=$9
shift 9
m=$1
case $w in
0) wn=Sun wf=Sunday ;;
1) wn=Mon wf=Monday ;;
2) wn=Tue wf=Tuesday ;;
3) wn=Wed wf=Wednesday ;;
4) wn=Thu wf=Thursday ;;
5) wn=Fri wf=Friday ;;
6) wn=Sat wf=Saturday ;;
*) pc_fail "%w is '$w', not 0-6" ;;
esac
[ "$a" = "$wn" ] || pc_fail "%a is '$a' for weekday number $w, expected $wn"
[ "$A" = "$wf" ] || pc_fail "%A is '$A' for weekday number $w, expected $wf"
if [ "$w" -eq 0 ]; then
	[ "$u" = 7 ] || pc_fail "%u is '$u' on a Sunday, expected 7"
else
	[ "$u" = "$w" ] || pc_fail "%u is '$u', %w is '$w'"
fi
inrange month "$m" 1 12
mi=$v
set -- Jan January Feb February Mar March Apr April May May Jun June Jul July Aug August Sep September Oct October Nov November Dec December
k=$((mi * 2 - 1))
eval "bn=\${$k}"
k=$((mi * 2))
eval "bf=\${$k}"
[ "$b" = "$bn" ] || pc_fail "%b is '$b' for month $m, expected $bn"
[ "$B" = "$bf" ] || pc_fail "%B is '$B' for month $m, expected $bf"
inrange day "$d" 1 31
inrange "%j" "$j" 1 366
inrange "%U" "$U" 0 53
inrange "%W" "$W" 0 53
inrange "%V" "$V" 1 53
inrange "%H" "$H" 0 23
hh=$v
inrange "%I" "$I" 1 12
ii=$v
inrange "%M" "$M" 0 59
inrange "%S" "$S" 0 60
inrange "%y" "$y" 0 99
[ "${#j}" -eq 3 ] || pc_fail "%j is '$j', not three digits"
[ "${#d}" -eq 2 ] || pc_fail "%d is '$d', not two digits"
if [ "$hh" -lt 12 ]; then
	[ "$p" = AM ] || pc_fail "%p is '$p' at hour $H, expected AM"
else
	[ "$p" = PM ] || pc_fail "%p is '$p' at hour $H, expected PM"
fi
want=$((hh % 12))
[ "$want" -ne 0 ] || want=12
[ "$ii" -eq "$want" ] || pc_fail "%I is '$I' at hour $H, expected $want"
case $d in
0?) [ "$e" = " ${d#0}" ] || pc_fail "%e is '$e' for %d '$d', expected a space and the digit" ;;
*) [ "$e" = "$d" ] || pc_fail "%e is '$e' for %d '$d'" ;;
esac
pc_pass
