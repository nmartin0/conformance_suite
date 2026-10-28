# date writes the day and month names, the meridiem and the %c format of the locale that LC_TIME (or LC_ALL, or LANG) names.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.date.010
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcloc.sh"
pc_begin XCU.date.010
LC_ALL=POSIX
export LC_ALL
pcl_d=pcl.$$
mkdir "$pcl_d" || pc_unresolved "cannot make a directory"
cd "$pcl_d" || pc_unresolved "cannot enter the directory"
trap 'cd ..; rm -rf "$pcl_d"' EXIT
pcloc_make
pcl_letter() {
	case $1 in
	0) pcl_l=a ;; 1) pcl_l=b ;; 2) pcl_l=c ;; 3) pcl_l=d ;; 4) pcl_l=e ;; 5) pcl_l=f ;;
	6) pcl_l=g ;; 7) pcl_l=h ;; 8) pcl_l=i ;; 9) pcl_l=j ;; 10) pcl_l=k ;; 11) pcl_l=l ;;
	esac
}
pcl_cnames() {
	case $1 in
	0) pcl_cd=Sunday pcl_ca=Sun ;; 1) pcl_cd=Monday pcl_ca=Mon ;; 2) pcl_cd=Tuesday pcl_ca=Tue ;;
	3) pcl_cd=Wednesday pcl_ca=Wed ;; 4) pcl_cd=Thursday pcl_ca=Thu ;; 5) pcl_cd=Friday pcl_ca=Fri ;;
	6) pcl_cd=Saturday pcl_ca=Sat ;;
	esac
}
pcl_cmonths() {
	case $1 in
	1) pcl_cm=January pcl_cb=Jan ;; 2) pcl_cm=February pcl_cb=Feb ;; 3) pcl_cm=March pcl_cb=Mar ;;
	4) pcl_cm=April pcl_cb=Apr ;; 5) pcl_cm=May pcl_cb=May ;; 6) pcl_cm=June pcl_cb=Jun ;;
	7) pcl_cm=July pcl_cb=Jul ;; 8) pcl_cm=August pcl_cb=Aug ;; 9) pcl_cm=September pcl_cb=Sep ;;
	10) pcl_cm=October pcl_cb=Oct ;; 11) pcl_cm=November pcl_cb=Nov ;; 12) pcl_cm=December pcl_cb=Dec ;;
	esac
}
# One date run gives the weekday number, month number and hour together with
# the names to check, so the names are compared with what the numbers say.
pcl_probe() {
	pcl_out=$(date '+%w %A %a %m %B %b %H %p' 2>/dev/null)
	set -- "$1" $pcl_out
	if [ $# -ne 9 ]; then
		printf 'date wrote [%s]' "$pcl_out"
		return 0
	fi
	pcl_k=$1 pcl_w=$2 pcl_A=$3 pcl_a=$4 pcl_m=${5#0} pcl_B=$6 pcl_b=$7 pcl_H=${8#0} pcl_p=$9
	pcl_got="$pcl_A $pcl_a $pcl_B $pcl_b $pcl_p"
	if [ "$pcl_k" = L ]; then
		pcl_letter "$pcl_w"
		pcl_wl=$pcl_l
		pcl_letter $((pcl_m - 1))
		pcl_ml=$pcl_l
		if [ "${pcl_H:-0}" -lt 12 ]; then pcl_ap=ante; else pcl_ap=post; fi
		pcl_exp="u${pcl_wl}day u${pcl_wl} mon${pcl_ml} m${pcl_ml} $pcl_ap"
		pcl_fmt='%Y.%m.%d %H:%M:%S'
	else
		pcl_cnames "$pcl_w"
		pcl_cmonths "$pcl_m"
		if [ "${pcl_H:-0}" -lt 12 ]; then pcl_ap=AM; else pcl_ap=PM; fi
		pcl_exp="$pcl_cd $pcl_ca $pcl_cm $pcl_cb $pcl_ap"
		pcl_fmt='%a %b %e %H:%M:%S %Y'
	fi
	pcl_cc=$(date "+$pcl_fmt|%c" 2>/dev/null)
	if [ "$pcl_got" != "$pcl_exp" ]; then
		printf 'names [%s] expected [%s]' "$pcl_got" "$pcl_exp"
	elif [ "${pcl_cc%%|*}" != "${pcl_cc#*|}" ]; then
		printf '%%c wrote [%s]' "$pcl_cc"
	else
		printf OK
	fi
}
pcl_want() {
	printf OK
}
pc_test
pcloc_matrix "$PCLOC" LC_TIME
pc_pass
