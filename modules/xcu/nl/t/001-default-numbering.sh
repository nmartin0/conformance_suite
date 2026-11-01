# By default nl numbers the non-empty lines, right-justified in six columns, followed by a tab.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nl.001
# MUTANT    nl.default_width5
# MUTANT    nl.default_width7
# MUTANT    nl.default_left
# MUTANT    nl.default_zero
# MUTANT    nl.default_sep_space
# MUTANT    nl.default_a
# MUTANT    nl.default_start0
# MUTANT    nl.default_incr2
# MUTANT    nl.stdin_unread
# MUTANT    nl.file_ignored
# MUTANT    nl.long_trunc
# MUTANT    nl.exit1
# MUTANT    nl.no_newline
# MUTANT    nl.text_first
# VARIANT   nl.delim_dropped
# VARIANT   nl.delim_echo
# VARIANT   nl.t_graph
. "$PC_CORE/pc.sh"
pc_begin XCU.nl.001
LC_ALL=POSIX
export LC_ALL
NL='
'
T=$(printf '\t')
numbered() {
	while IFS= read -r l; do
		case $l in
		*"$1"*) printf '%s\n' "$l" ;;
		esac
	done <"$2"
}
x10=xxxxxxxxxx
x100=$x10$x10$x10$x10$x10$x10$x10$x10$x10$x10
x1500=$x100$x100$x100$x100$x100$x100$x100$x100$x100$x100$x100$x100$x100$x100$x100
printf 'a\n\nb\nc\n%s\n' "$x1500" >in || pc_fail "cannot create input"
want="     1${T}a${NL}     2${T}b${NL}     3${T}c${NL}     4${T}${x1500}${NL}x"
pc_test
nl in >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "nl gave status $rc"
[ -f out ] || pc_fail "no output was captured"
got=$(numbered "$T" out; printf x)
[ "$got" = "$want" ] || pc_fail "the numbered lines are not '%6d', tab, text (empty lines skipped)"
nl <in >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "nl on standard input gave status $rc"
got=$(numbered "$T" out; printf x)
[ "$got" = "$want" ] || pc_fail "nl did not read standard input"
pc_pass
