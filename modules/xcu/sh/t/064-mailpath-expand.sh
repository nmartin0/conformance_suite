# The message of a MAILPATH entry undergoes parameter expansion when it is written.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh.064
. "$PC_CORE/pc.sh"
pc_begin XCU.sh.064
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
printf 'PCMV=expd\nsleep 1; echo m >>pc.mail\necho\nexit\n' >pc.in || pc_unresolved "write"
# The message after % is the one for a change of modification time; for a
# file that is created the message is not stated, so the file exists, with
# an old time, before the shell starts.
: >pc.mail && touch -t 200001010000 pc.mail || pc_unresolved "cannot create the mail file"
mp="$PWD/pc.mail%tok-\$PCMV"
pc_test
ish pc.in "MAILPATH=$mp" MAILCHECK=0
o=$(tr -d '\r' <pty.out)
printf '%s\n' "$o" | grep -F 'tok-expd' >/dev/null || pc_fail "the message was not expanded: $o"
pc_pass
