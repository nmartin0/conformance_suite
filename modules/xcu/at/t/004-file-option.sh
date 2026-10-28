# at -f.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.at.004
. "$PC_CORE/pc.sh"
pc_begin XCU.at.004
LC_ALL=C; export LC_ALL
TZ=UTC0; export TZ
d=pcat.$$
jobs=
jid=
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
# listed: ids = the job IDs written by at -l, each followed by a blank
listed() {
	ids=
	at -l >"$d/l" 2>/dev/null </dev/null
	while IFS= read -r ll; do
		ids="$ids${ll%%"$tab"*} "
	done <"$d/l"
}
# newids OLD: new = the IDs now listed that are not in OLD (jobs may also end)
newids() {
	new=
	listed
	for ni in $ids; do
		case " $1" in
		*" $ni "*) ;;
		*) new="$new $ni" ;;
		esac
	done
}
printf 'true\n' >"$d/cmds" || pc_fail "cannot write the command file"
listed
before=$ids
pc_test
at -f "$d/cmds" -t 203512091200 >"$d/so" 2>"$d/se" </dev/null
rc=$?
[ "$rc" -ne 0 ] || {
	while IFS= read -r sl; do
		set -- $sl
		[ "$#" -ge 3 ] && [ "$1" = job ] && { jobs="$jobs $2"; jid=$2; }
	done <"$d/se"
}
[ "$rc" -eq 0 ] || pc_unresolved "at -f with a good file failed (status $rc): the user may not be allowed to use at"
[ -n "$jid" ] || pc_fail "at -f with a good file printed no job line"
listed
case " $ids" in
*" $jid "*) ;;
*) pc_fail "the job $jid of at -f with a good file is not listed" ;;
esac
after=$ids
at -f "$d/nosuchfile" -t 203512091200 >"$d/so" 2>"$d/se" </dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "at -f of a missing file exited with status 0"
newids "$after"
[ -z "$new" ] || pc_fail "at -f of a missing file scheduled a job ($new)"
pc_pass
