# With set -o vi, the escape character, a motion to the line start and x delete characters of the command line being edited, and the edited line is what runs
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh.024
. "$PC_CORE/pc.sh"
pc_begin XCU.sh.024
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

{ printf "set -o vi\\n"; printf 'xecho Q1Q\0330x\n'; printf 'exit\n'; } >pc.in || pc_unresolved "write"
pc_test
ish pc.in
tr -d '\r' <pty.out | grep -q '^Q1Q$' || pc_fail "the edited command did not run: $(tr -d '\r' <pty.out)"
pc_pass
