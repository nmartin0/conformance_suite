# at -l format and selection.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.at.002
. "$PC_CORE/pc.sh"
pc_begin XCU.at.002
LC_ALL=C; export LC_ALL
TZ=UTC0; export TZ
d=pcat.$$
jobs=
cleanup() {
	for j in $jobs; do
		at -r "$j" >/dev/null 2>&1 </dev/null
	done
	rm -rf "$d"
}
trap cleanup EXIT
# a signal ends the test through the EXIT trap, so the jobs are removed
trap 'exit 1' HUP INT TERM
mkdir "$d" || pc_fail "cannot make $d"
tab=$(printf '\t')
# submit ARGS: schedule the command true. Sets jid and jdate (the words of
# the scheduled time as written in the message) and adds the job to $jobs.
# Returns 1 when at fails and 2 when the message has no job line.
submit() {
	printf 'true\n' | at "$@" >"$d/so" 2>"$d/se"
	srcode=$?
	jid= jdate=
	[ "$srcode" -eq 0 ] || return 1
	while IFS= read -r sl; do
		set -- $sl
		if [ "$#" -ge 3 ] && [ "$1" = job ]; then
			jid=$2
			shift 3
			jdate=$*
		fi
	done <"$d/se"
	[ -n "$jid" ] || return 2
	jobs="$jobs $jid"
	return 0
}
# nlist: n = number of lines written by at -l
nlist() {
	at -l >"$d/l" 2>/dev/null </dev/null
	n=0
	while IFS= read -r nl_; do
		n=$((n + 1))
	done <"$d/l"
}
submit -t 203512091200 || pc_unresolved "setup: at could not submit a job (status $srcode) [stderr: $(tr '\n' ' ' <"$d/se" | head -c 160); stdout: $(tr '\n' ' ' <"$d/so" | head -c 80)]"
a=$jid
submit -t 203512101300 || pc_unresolved "setup: at could not submit a second job (status $srcode) [stderr: $(tr '\n' ' ' <"$d/se" | head -c 160); stdout: $(tr '\n' ' ' <"$d/so" | head -c 80)]"
b=$jid
pc_test
at -l >"$d/l" 2>/dev/null </dev/null || pc_fail "at -l failed"
fa=no fb=no
while IFS= read -r l; do
	id=${l%%"$tab"*}
	case $l in
	*"$tab"*) ;;
	*) pc_fail "listing line '$l' has no tab" ;;
	esac
	rest=${l#*"$tab"}
	case $rest in
	*"$tab"*) pc_fail "listing line '$l' has more than one tab" ;;
	esac
	set -- $rest
	if [ "$id" = "$a" ]; then
		fa=yes
		[ "$*" = "Sun Dec 9 12:00:00 2035" ] || pc_fail "job $a is listed with '$rest'"
	fi
	if [ "$id" = "$b" ]; then
		fb=yes
		[ "$*" = "Mon Dec 10 13:00:00 2035" ] || pc_fail "job $b is listed with '$rest'"
	fi
done <"$d/l"
[ "$fa" = yes ] && [ "$fb" = yes ] || pc_fail "at -l did not list both jobs ($a and $b)"
at -l "$a" >"$d/l" 2>/dev/null </dev/null || pc_fail "at -l $a failed"
n=0
while IFS= read -r l; do
	n=$((n + 1))
	id=${l%%"$tab"*}
	[ "$id" = "$a" ] || pc_fail "at -l $a listed '$l'"
done <"$d/l"
[ "$n" -eq 1 ] || pc_fail "at -l $a wrote $n lines, expected 1"
pc_pass
