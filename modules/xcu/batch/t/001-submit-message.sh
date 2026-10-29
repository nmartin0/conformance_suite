# batch: the message on standard error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.batch.001
. "$PC_CORE/pc.sh"
pc_begin XCU.batch.001
LC_ALL=C; export LC_ALL
TZ=UTC0; export TZ
d=pcbatch.$$
jid=
cleanup() {
	[ -n "$jid" ] && at -r "$jid" >/dev/null 2>&1 </dev/null
	rm -rf "$d"
}
trap cleanup EXIT
# a signal ends the test through the EXIT trap, so the jobs are removed
trap 'exit 1' HUP INT TERM
mkdir "$d" || pc_fail "cannot make $d"
pc_test
printf 'true\n' | batch >"$d/so" 2>"$d/se"
rc=$?
while IFS= read -r l; do
	set -- $l
	if [ "$#" -ge 3 ] && [ "$1" = job ]; then
		jid=$2
		shift 3
		jdate=$*
		jline=$l
	fi
done <"$d/se"
[ "$rc" -eq 0 ] || pc_unresolved "batch exited with status $rc: the user may not be allowed to use it [stderr: $(tr '\n' ' ' <"$d/se" | head -c 160); stdout: $(tr '\n' ' ' <"$d/so" | head -c 80)]"
[ -n "$jid" ] || pc_fail "standard error has no line 'job ID at DATE' [stderr: $(tr '\n' ' ' <"$d/se" | head -c 160); stdout: $(tr '\n' ' ' <"$d/so" | head -c 80)]"
case $jid in
*[!A-Za-z0-9.]*) pc_fail "the job ID '$jid' has characters other than letters, digits and periods" ;;
esac
set -- $jdate
[ "$#" -eq 5 ] || pc_fail "the date '$jdate' does not have five words"
case $1 in Sun | Mon | Tue | Wed | Thu | Fri | Sat) ;; *) pc_fail "weekday '$1' in the date" ;; esac
case $2 in Jan | Feb | Mar | Apr | May | Jun | Jul | Aug | Sep | Oct | Nov | Dec) ;; *) pc_fail "month '$2' in the date" ;; esac
case $3 in [1-9] | [12][0-9] | 3[01]) ;; *) pc_fail "day '$3' in the date" ;; esac
case $4 in [0-2][0-9]:[0-5][0-9]:[0-6][0-9]) ;; *) pc_fail "time '$4' in the date" ;; esac
case $5 in [0-9][0-9][0-9][0-9]) ;; *) pc_fail "year '$5' in the date" ;; esac
[ -s "$d/so" ] && pc_fail "batch wrote to standard output"
pc_pass
