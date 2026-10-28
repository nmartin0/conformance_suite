# A working directory counts as used and gets the c character.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.fuser.005
. "$PC_CORE/pc.sh"
pc_begin XCU.fuser.005
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
( cd "$d" && exec sleep 301 ) >/dev/null 2>&1 </dev/null &
bg=$!
pc_test
waitfor "$d" || pc_fail "fuser did not list the process $bg that has $d as working directory"
fuser "$d" >co 2>&1 </dev/null || pc_fail "fuser failed"
line=$(cat co)
found=no
for w in $line; do
	case $w in
	"$bg"*)
		use=${w#"$bg"}
		case $use in
		*c*) found=yes ;;
		*) pc_fail "use characters '$use' of the entry for $bg do not include c" ;;
		esac
		;;
	esac
done
[ "$found" = yes ] || pc_fail "no entry for the process $bg in '$line'"
pc_pass
