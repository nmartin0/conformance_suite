# The command history kept in the file named by HISTFILE survives between interactive sessions: a command typed in one session is listed by fc -l in a later session that uses the same file.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh.058
. "$PC_CORE/pc.sh"
pc_begin XCU.sh.058
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
printf 'echo persist1\nexit\n' >pc.in1 || pc_unresolved "write"
printf 'fc -l -n -50\nexit\n' >pc.in2 || pc_unresolved "write"
pc_test
ish pc.in1
ish pc.in2
o=$(tr -d '\r' <pty.out)
tab=$(printf '\t')
n=$(printf '%s\n' "$o" | grep -c -x -F -e "${tab}echo persist1")
[ "$n" -ge 1 ] || pc_fail "the earlier session's command is not in the history: $o"
pc_pass
