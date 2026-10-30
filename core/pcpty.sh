# pcpty.sh - helpers for tests that need a terminal. Source after pc.sh.
#
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# The driver builds core/pcpty.c and exports its path as PC_PTY; the helper
# runs a command on a private pseudo-terminal, so no real terminal is used.
#
#     pc_pty_need                       UNRESOLVED when there is no helper
#     pc_pty_run [-c] [-i INPUT] SCRIPT run the sh script file SCRIPT on the
#                                       terminal; output goes to pty.out; the
#                                       status is left in pc_pty_rc; -c makes
#                                       it UNRESOLVED when the terminal did not
#                                       become the controlling terminal
#     pc_pty_term                       set PCT to a terminal type that the
#                                       system knows (UNRESOLVED if none)

pc_pty_need() {
	[ -n "${PC_PTY:-}" ] && [ -x "$PC_PTY" ] || pc_unresolved "no pseudo-terminal helper"
}

pc_pty_run() {
	pc_pty_in=
	pc_pty_c=
	if [ "$1" = -c ]; then pc_pty_c=-c; shift; fi
	if [ "$1" = -i ]; then pc_pty_in=$2; shift 2; fi
	if [ -n "$pc_pty_in" ]; then
		"$PC_PTY" $pc_pty_c -t 30 -i "$pc_pty_in" -o pty.out -- "${PC_SH:-sh}" "$1"
	else
		"$PC_PTY" $pc_pty_c -t 30 -o pty.out -- "${PC_SH:-sh}" "$1"
	fi
	pc_pty_rc=$?
	[ "$pc_pty_rc" -ne 125 ] || pc_unresolved "cannot set up a pseudo-terminal (or, with -c, it did not become the controlling terminal)"
	[ "$pc_pty_rc" -ne 124 ] || pc_unresolved "the command on the terminal did not finish"
}

pc_pty_term() {
	PCT=
	for pc_t in vt100 xterm ansi linux dumb; do
		if command tput -T "$pc_t" clear >/dev/null 2>&1 </dev/null; then PCT=$pc_t; break; fi
	done
	[ -n "$PCT" ] || pc_unresolved "no known terminal type"
}

# pc_vi_run INPUT [VI-ARGS...]: run vi on the terminal typing the lines of
# INPUT (one line per step, ESC written as \033 by printf); the status of vi
# is left in pc_pty_rc. PCT must be set (pc_pty_term). The first line is typed
# only after vi has drawn its screen and been quiet for a moment.
pc_vi_run() {
	pc_vi_in=$1; shift
	"$PC_PTY" -r -t 15 -d 400 -i "$pc_vi_in" -o pty.out -- env "TERM=$PCT" \
	    "${PC_SH:-sh}" -c 'stty rows 24 cols 80; exec vi "$@"' vi "$@"
	pc_pty_rc=$?
	[ "$pc_pty_rc" -ne 125 ] || pc_unresolved "cannot set up a pseudo-terminal (or, with -c, it did not become the controlling terminal)"
	[ "$pc_pty_rc" -ne 124 ] || pc_fail "vi did not finish"
}
