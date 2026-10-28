# With set -b the shell writes the completion message of a background job as soon as it completes, even while a foreground command is still running.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-jobs.008
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-jobs.008
LC_ALL=C; export LC_ALL
unset MAIL MAILPATH MAILCHECK
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
printf 'set -b\nsleep 1 &\nsh -c \047sleep 3; echo FGMID\047\necho END\nexit\n' >pc.in || pc_unresolved "write"
pc_test
ish pc.in
o=$(tr -d '\r' <pty.out)
ld=$(printf '%s\n' "$o" | grep -n 'Done' | head -n 1 | cut -d: -f1)
lm=$(printf '%s\n' "$o" | grep -n -x -F FGMID | head -n 1 | cut -d: -f1)
[ -n "$ld" ] || pc_fail "no completion message: $o"
[ -n "$lm" ] || pc_unresolved "the foreground command printed nothing"
[ "$ld" -lt "$lm" ] || pc_fail "the message came after the foreground command ended (line $ld, FGMID at $lm)"
pc_pass
