# With set -o vi, v in command mode invokes vi on the current command line in a temporary file, and when vi exits the commands in the file are executed.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh.057
. "$PC_CORE/pc.sh"
pc_begin XCU.sh.057
LC_ALL=C; export LC_ALL
unset MAIL MAILPATH MAILCHECK
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
. "$PC_CORE/pcpty.sh"

pc_pty_need
pc_pty_term
# ish INPUT [ENV=VAL...]: run an interactive shell on a terminal with INPUT typed; output in pty.out
ish() {
	ish_in=$1; shift
	"$PC_PTY" -t 25 -d 600 -i "$ish_in" -o pty.out -- env HISTFILE="$PWD/pc.hist" ENV= PS1='PCP> ' TERM=$PCT "$@" \
	    "${PC_SH:-sh}" -c 'stty erase "^H" kill "^U" werase "^W" intr "^C" eof "^D" susp "^Z" 2>/dev/null; exec "$0" -i' "${PC_SH:-sh}"
	ish_rc=$?
	[ "$ish_rc" -ne 125 ] || pc_unresolved "cannot set up a pseudo-terminal"
	[ "$ish_rc" -ne 124 ] || pc_fail "the interactive shell did not finish"
}
# chk N TEXT: TEXT is a whole line of the output exactly N times
chk() {
	n=$(printf '%s\n' "$o" | grep -c -x -F -e "$2")
	[ "$n" -eq "$1" ] || pc_fail "the line '$2' appeared $n times, wanted $1: $o"
}
printf 'set -o vi\necho vt$((1+1))est\033v\n\033\n\033\n:wq\n' >pc.in || pc_unresolved "write"
pc_test
ish pc.in
o=$(tr -d '\r' <pty.out)
printf '%s\n' "$o" | grep -F -e 'vt2est' >/dev/null || pc_fail "'vt2est' is not in the output: $o"
pc_pass
