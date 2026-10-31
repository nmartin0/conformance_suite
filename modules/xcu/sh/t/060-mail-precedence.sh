# When MAILPATH and MAIL are both set, only the files of MAILPATH are watched: a change to the MAIL file adds nothing to what the user is told.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh.060
. "$PC_CORE/pc.sh"
pc_begin XCU.sh.060
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
printf 'echo x >m1\necho y >m2\necho z\nexit\n' >pc.ina || pc_unresolved "write"
printf 'echo x >m1\necho y >m2\necho z\nexit\n' >pc.inb || pc_unresolved "write"
pc_test
ish pc.ina "MAIL=$PWD/m1" "MAILPATH=$PWD/m2%MPMSG" MAILCHECK=0
oa=$(tr -d '\r' <pty.out)
rm -f m1 m2
ish pc.inb "MAIL=$PWD/m9" "MAILPATH=$PWD/m2%MPMSG" MAILCHECK=0
ob=$(tr -d '\r' <pty.out)
printf '%s\n' "$oa" | grep -F MPMSG >/dev/null || pc_fail "no MAILPATH message: $oa"
# What the user is told, without the lines that show a typed command: a
# line typed before the shell reads it can be echoed twice (by the terminal
# and again by the shell), so those lines depend on timing.
told() {
	printf '%s\n' "$1" | grep -v -F -e 'echo x >m1' -e 'echo y >m2' -e 'echo z' -e 'exit'
}
ta=$(told "$oa")
tb=$(told "$ob")
[ "$ta" = "$tb" ] || pc_fail "the MAIL file was watched although MAILPATH is set: told '$ta' against '$tb'"
pc_pass
