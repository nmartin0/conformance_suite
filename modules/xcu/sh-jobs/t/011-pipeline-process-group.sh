# The processes of a background pipeline share one process group.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-jobs.011
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-jobs.011
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
printf 'sleep 30 | sleep 30 &\njobs -p >pc.jp\necho G$(cat pc.jp)X L$(ps -o pgid= -p $! | tr -d " ")X\nkill %%1\nsleep 1\nexit\n' >pc.in || pc_unresolved "write"
pc_test
ish pc.in
o=$(tr -d '\r' <pty.out)
l=$(printf '%s\n' "$o" | grep -E '^G[0-9]+X L[0-9]+X$' | head -n 1)
[ -n "$l" ] || pc_unresolved "could not read the process group numbers: $o"
g=${l#G}; g=${g%%X*}
p=${l##*L}; p=${p%X}
[ "$g" = "$p" ] || pc_fail "the job's process group is $g but the last process is in group $p"
pc_pass
