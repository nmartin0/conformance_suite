# Standard error starts with the pathname and a colon.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.fuser.002
. "$PC_CORE/pc.sh"
pc_begin XCU.fuser.002
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
nl='
'
sleep 301 <"$d/f" >/dev/null 2>&1 &
bg=$!
rp=$(pwd -P)/$d/f
pc_test
waitfor "$d/f" || pc_fail "fuser did not list the process $bg that holds the file"
e=$(cat fe; printf x)
e=${e%x}
case $e in
"$d/f:"* | "$rp:"*) ;;
*) pc_fail "standard error '$e' does not start with the pathname and a colon" ;;
esac
case $e in
*"$nl") ;;
*) pc_fail "standard error does not end with a newline" ;;
esac
pc_pass
