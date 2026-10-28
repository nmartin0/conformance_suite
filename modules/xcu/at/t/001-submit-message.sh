# Submission message of at -t.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.at.001
. "$PC_CORE/pc.sh"
pc_begin XCU.at.001
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
pc_test
submit -t 203512091200
rc=$?
[ "$rc" -ne 1 ] || pc_unresolved "at refused the job (status $srcode): the user may not be allowed to use it"
[ "$rc" -eq 0 ] || pc_fail "standard error has no line 'job ID at DATE'"
case $jid in
'' | *[!A-Za-z0-9.]*) pc_fail "the job ID '$jid' has characters other than letters, digits and periods" ;;
esac
[ "$jdate" = "Sun Dec 9 12:00:00 2035" ] || pc_fail "the scheduled time is '$jdate', expected 'Sun Dec 9 12:00:00 2035'"
[ -s "$d/so" ] && pc_fail "at wrote to standard output"
pc_pass
