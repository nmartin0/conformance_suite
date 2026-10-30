# When the foreground job is stopped from the terminal (the suspend character), the shell regains control and reports it as stopped; jobs lists it as stopped; bg continues it so that jobs then lists it as running
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-jobs.004
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-jobs.004
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
. "$PC_CORE/pcpty.sh"

pc_pty_need
# ish INPUT [ENV=VAL...]: run an interactive shell on a terminal with INPUT typed; output in pty.out
ish() {
	ish_in=$1; shift
	"$PC_PTY" -c -r -t 25 -d 400 -i "$ish_in" -o pty.out -- env HISTFILE="$PWD/pc.hist" ENV= PS1='PCP> ' "$@" \
	    "${PC_SH:-sh}" -c 'stty susp "^Z" 2>/dev/null; exec "$0" -i' "${PC_SH:-sh}"
	ish_rc=$?
	[ "$ish_rc" -ne 125 ] || pc_unresolved "cannot set up a pseudo-terminal (or it did not become the controlling terminal)"
	[ "$ish_rc" -ne 124 ] || pc_fail "the interactive shell did not finish"
}

{ printf 'sleep 30\n'; printf '\032\n'; printf 'jobs\nbg\njobs\nkill %%1\nwait\nexit\n'; } >pc.in || pc_unresolved "write"
pc_test
ish pc.in
o=$(tr -d '\r' <pty.out)
n=$(printf '%s\n' "$o" | grep -c -E 'Stopped|Suspended')
[ "$n" -ge 2 ] || pc_fail "expected the stop message and the jobs listing to show the job stopped (seen $n): $o"
case $o in *Running*) : ;; *) pc_fail "after bg, jobs did not list the job as running: $o" ;; esac
pc_pass
