# Shapes of etime, time, nice, vsz and pcpu.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ps.011
. "$PC_CORE/pc.sh"
pc_begin XCU.ps.011
LC_ALL=C; export LC_ALL
get() { # get FIELD: v = the value for the shell running the test
	ps -o "$1=" -p "$$" >out 2>err </dev/null || pc_fail "ps -o $1= failed"
	read -r v <out
}
digits() { case $1 in '' | *[!0-9]*) return 1 ;; esac; }
# clock FORM VALUE: FORM 1 = [[dd-]hh:]mm:ss, FORM 2 = [dd-]hh:mm:ss
clock() {
	x=$2
	case $x in
	*-*)
		d=${x%%-*}
		digits "$d" || return 1
		x=${x#*-}
		case $x in
		[0-9][0-9]:[0-9][0-9]:[0-9][0-9]) return 0 ;;
		esac
		return 1
		;;
	esac
	case $x in
	[0-9][0-9]:[0-9][0-9]:[0-9][0-9]) return 0 ;;
	[0-9][0-9]:[0-9][0-9]) [ "$1" -eq 1 ] && return 0 ;;
	esac
	return 1
}
pc_test
get etime
[ "$v" = "-" ] || clock 1 "$v" || pc_fail "etime '$v' is not [[dd-]hh:]mm:ss"
get time
[ "$v" = "-" ] || clock 2 "$v" || pc_fail "time '$v' is not [dd-]hh:mm:ss"
get nice
case $v in
- | [0-9]*) ;;
-[0-9]*) ;;
*) pc_fail "nice '$v' is not a decimal integer" ;;
esac
case $v in -*) w=${v#-} ;; *) w=$v ;; esac
[ "$v" = "-" ] || digits "$w" || pc_fail "nice '$v' is not a decimal integer"
get vsz
[ "$v" = "-" ] || digits "$v" || pc_fail "vsz '$v' is not a decimal integer"
get pcpu
case $v in
- ) ;;
'' | *[!0-9.]* | *.*.*) pc_fail "pcpu '$v' is not a decimal number" ;;
esac
pc_pass
