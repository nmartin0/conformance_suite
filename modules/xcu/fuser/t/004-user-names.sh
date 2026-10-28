# -u adds the user in parentheses.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.fuser.004
. "$PC_CORE/pc.sh"
pc_begin XCU.fuser.004
LC_ALL=C; export LC_ALL
d=pcfuser.$$
bg=
cleanup() {
	[ -n "$bg" ] && { kill "$bg" 2>/dev/null; wait "$bg" 2>/dev/null; }
	rm -rf "$d"
}
trap cleanup EXIT
mkdir "$d" || pc_fail "cannot make $d"
: >"$d/f" || pc_fail "cannot make a file"
# haspid FILE PID: PID is one of the whitespace separated numbers in FILE.
haspid() {
	for w in $(cat "$1"); do
		[ "$w" = "$2" ] && return 0
	done
	return 1
}
# waitfor ARGS: run fuser until its standard output lists the child (8 tries).
waitfor() {
	t=0
	while :; do
		fuser "$@" >fo 2>fe </dev/null
		frc=$?
		haspid fo "$bg" && return 0
		t=$((t + 1))
		if [ "$t" -ge 8 ]; then
			# a user may need privileges to run fuser at all
			if [ "$frc" -ne 0 ] && [ ! -s fo ]; then
				pc_unresolved "fuser exited with status $frc and listed nothing (it may need privileges)"
			fi
			return 1
		fi
		sleep 1
	done
}
sleep 301 <"$d/f" >/dev/null 2>&1 &
bg=$!
nm=$(id -urn 2>/dev/null)
num=$(id -ur)
pc_test
waitfor "$d/f" || pc_fail "fuser did not list the process $bg that holds the file"
fuser -u "$d/f" >co 2>&1 </dev/null || pc_fail "fuser -u failed"
line=$(cat co)
found=no
for w in $line; do
	case $w in
	"$bg"*)
		rest=${w#"$bg"}
		use=${rest%%\(*}
		case $use in
		*[!A-Za-z]*) pc_fail "use characters '$use' in '$w' are not alphabetic" ;;
		esac
		case $rest in
		*\(*\)) ;;
		*) pc_fail "entry '$w' has no user in parentheses" ;;
		esac
		u=${rest#*\(}
		u=${u%\)}
		if [ "$u" = "$nm" ] || [ "$u" = "$num" ]; then found=yes; else pc_fail "user in '$w' is '$u', expected '$nm' or '$num'"; fi
		;;
	esac
done
[ "$found" = yes ] || pc_fail "fuser -u gave no entry for $bg in '$line'"
pc_pass
