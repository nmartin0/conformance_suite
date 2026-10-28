# The holder's pid on standard output.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.fuser.001
. "$PC_CORE/pc.sh"
pc_begin XCU.fuser.001
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
pc_test
waitfor "$d/f" || pc_fail "fuser did not list the process $bg that holds the file"
[ "$frc" -eq 0 ] || pc_fail "fuser exited with status $frc"
v=$(cat fo)
case $v in
" "*) ;;
*) pc_fail "standard output '$v' does not start with a blank" ;;
esac
case $v in
*[!0-9' ']*) pc_fail "standard output '$v' holds something other than blanks and digits" ;;
esac
fuser -f "$d/f" >fo 2>fe </dev/null
haspid fo "$bg" || pc_fail "fuser -f did not list the process $bg"
pc_pass
