# One output line when both streams go to one file.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.fuser.003
. "$PC_CORE/pc.sh"
pc_begin XCU.fuser.003
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
rp=$(pwd -P)/$d/f
pc_test
waitfor "$d/f" || pc_fail "fuser did not list the process $bg that holds the file"
fuser "$d/f" >co 2>&1 </dev/null || pc_fail "fuser failed"
n=0 line=
while IFS= read -r l; do
	n=$((n + 1))
	line=$l
done <co
[ "$n" -eq 1 ] || pc_fail "the combined output has $n lines, expected 1"
case $line in
"$d/f:"*) rest=${line#"$d/f:"} ;;
"$rp:"*) rest=${line#"$rp:"} ;;
*) pc_fail "the line '$line' does not start with the pathname and a colon" ;;
esac
found=no
for w in $rest; do
	num=${w%%[!0-9]*}
	use=${w#"$num"}
	[ -n "$num" ] || pc_fail "entry '$w' does not start with a process ID"
	case $use in
	*[!A-Za-z]*) pc_fail "use characters '$use' of entry '$w' are not alphabetic" ;;
	esac
	[ "$num" = "$bg" ] && found=yes
done
[ "$found" = yes ] || pc_fail "the combined line '$line' has no entry for $bg"
case $rest in
" "*) ;;
*) pc_fail "an entry is not preceded by a blank in '$rest'" ;;
esac
pc_pass
