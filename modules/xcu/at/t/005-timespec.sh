# timespec forms.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.at.005
. "$PC_CORE/pc.sh"
pc_begin XCU.at.005
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
chk() { # chk "expected date" ARGS...
	exp=$1
	shift
	submit "$@"
	rc=$?
	[ "$rc" -ne 1 ] || pc_unresolved "at refused 'at $*' (status $srcode)"
	[ "$rc" -eq 0 ] || pc_fail "at $*: no job line on standard error"
	[ "$jdate" = "$exp" ] || pc_fail "at $* scheduled '$jdate', expected '$exp'"
}
pc_test
chk "Sun Dec 9 12:00:00 2035" noon Dec 9, 2035
chk "Mon Dec 10 00:00:00 2035" midnight Dec 10, 2035
chk "Sun Dec 9 23:59:00 2035" 23:59 Dec 9, 2035
chk "Sun Dec 9 23:30:00 2035" 11:30 pm Dec 9, 2035
chk "Sun Dec 9 00:05:00 2035" 12:05 am Dec 9, 2035
chk "Sun Dec 16 12:00:00 2035" 12:00 Dec 9, 2035 + 1 week
chk "Mon Dec 10 12:00:00 2035" 12:00 Dec 9, 2035 next day
chk "Tue Dec 11 12:00:00 2035" 12:00 Dec 9, 2035 + 2 days
TZ=XYZ5
export TZ
chk "Sun Dec 9 07:00:00 2035" 12:00 utc Dec 9, 2035
pc_pass
