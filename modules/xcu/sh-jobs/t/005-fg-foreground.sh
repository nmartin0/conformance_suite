# fg brings a background job into the foreground: the shell waits for it, writes its command line, and continues with the next command only after the job ends
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-jobs.005
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-jobs.005
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
	"$PC_PTY" -t 25 -d 400 -i "$ish_in" -o pty.out -- env HISTFILE="$PWD/pc.hist" ENV= PS1='PCP> ' "$@" \
	    "${PC_SH:-sh}" -c 'stty susp "^Z" 2>/dev/null; exec "$0" -i' "${PC_SH:-sh}"
	ish_rc=$?
	[ "$ish_rc" -ne 125 ] || pc_unresolved "cannot set up a pseudo-terminal"
	[ "$ish_rc" -ne 124 ] || pc_fail "the interactive shell did not finish"
}

printf '%s\n' 'sleep 3 &' 'fg' 'echo FGDONE$((1+1))' 'exit' >pc.in || pc_unresolved "write"
pc_test
ish pc.in
o=$(tr -d '\r' <pty.out)
n=$(printf '%s\n' "$o" | grep -c 'sleep 3')
[ "$n" -ge 2 ] || pc_fail "fg did not write the command line: $o"
case $o in *FGDONE2*) : ;; *) pc_fail "no output of the command after fg: $o" ;; esac
pc_pass
