# nl -h and -f choose which header and footer lines are numbered, with the same types as -b, independently of each other and of -b.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nl.012
# MUTANT    nl.h_ignored
# MUTANT    nl.f_ignored
# MUTANT    nl.h_as_f
# MUTANT    nl.h_sets_body
# MUTANT    nl.f_sets_header
# MUTANT    nl.b_sets_all
# MUTANT    nl.h_t_as_a
# MUTANT    nl.h_a_as_t
# MUTANT    nl.h_p_as_t
# MUTANT    nl.f_t_as_a
# MUTANT    nl.f_a_as_t
# MUTANT    nl.f_p_as_t
# VARIANT   nl.delim_dropped
# VARIANT   nl.delim_echo
# VARIANT   nl.t_graph
#
# Only which lines are numbered is looked at (their text, in brackets, in
# output order), not the numbers.
. "$PC_CORE/pc.sh"
pc_begin XCU.nl.012
LC_ALL=POSIX
export LC_ALL
NL='
'
sel() {
	nl "$@" in 2>/dev/null </dev/null | sed -n 's/^ *[0-9][0-9]*|\(.*\)$/[\1]/p'
}
printf '%s\n' '\:\:\:' H1 '' '\:\:' B1 '' '\:' F1 '' >in || pc_fail "cannot create input"
pc_test
chk() {
	# chk WANT ARGS...
	w=$1
	shift
	got=$(sel -s '|' "$@"; printf x)
	[ "$got" = "${w}x" ] || pc_fail "nl $* numbered the wrong lines"
}
[ -f in ] || pc_fail "no input"
chk "[B1]$NL"
chk "[H1]$NL[]$NL[B1]$NL" -h a
chk "[H1]$NL[B1]$NL" -h t
chk "[B1]$NL" -h n
chk "[]$NL[B1]$NL" -h 'p^$'
chk "[H1]$NL[B1]$NL" -h 'pH'
chk "[B1]$NL[F1]$NL[]$NL" -f a
chk "[B1]$NL[F1]$NL" -f t
chk "[B1]$NL" -f n
chk "[B1]$NL[]$NL" -f 'p^$'
chk "[B1]$NL[F1]$NL" -f 'pF'
chk "[B1]$NL[]$NL" -b a
chk "[H1]$NL[]$NL[B1]$NL[]$NL[F1]$NL[]$NL" -h a -b a -f a
pc_pass
