# at -r and distinct IDs.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.at.003
. "$PC_CORE/pc.sh"
pc_begin XCU.at.003
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
submit -t 203512091200 || pc_unresolved "setup: at could not submit a job (status $srcode)"
a=$jid
submit -t 203512101300 || pc_unresolved "setup: at could not submit a second job (status $srcode)"
b=$jid
pc_test
[ "$a" != "$b" ] || pc_fail "two jobs got the same ID $a"
at -r "$a" >"$d/o" 2>"$d/e" </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "at -r $a exited with status $rc"
at -l >"$d/l" 2>/dev/null </dev/null
fa=no fb=no
while IFS= read -r l; do
	id=${l%%"$tab"*}
	[ "$id" = "$a" ] && fa=yes
	[ "$id" = "$b" ] && fb=yes
done <"$d/l"
[ "$fa" = no ] || pc_fail "job $a is still listed after at -r"
[ "$fb" = yes ] || pc_fail "job $b disappeared when job $a was removed"
pc_pass
