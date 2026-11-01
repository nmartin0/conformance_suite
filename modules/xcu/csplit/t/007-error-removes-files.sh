# When csplit fails it exits nonzero with a diagnostic and, without -k, leaves none of the files it made.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.csplit.007
# MUTANT    csplit.nomatch_status0
# MUTANT    csplit.nomatch_keeps
# MUTANT    csplit.nomatch_no_diag
# MUTANT    csplit.nomatch_diag_stdout
# MUTANT    csplit.nomatch_removes_some
# MUTANT    csplit.nomatch_removes_preexisting
# MUTANT    csplit.past_status0
# MUTANT    csplit.past_keeps
# MUTANT    csplit.past_no_diag
# MUTANT    csplit.past_diag_stdout
# MUTANT    csplit.past_removes_some
# MUTANT    csplit.past_removes_preexisting
# MUTANT    csplit.back_status0
# MUTANT    csplit.back_keeps
# MUTANT    csplit.back_no_diag
# MUTANT    csplit.back_diag_stdout
# MUTANT    csplit.back_removes_some
# MUTANT    csplit.back_removes_preexisting
# MUTANT    csplit.repeat_status0
# MUTANT    csplit.repeat_keeps
# MUTANT    csplit.repeat_no_diag
# MUTANT    csplit.repeat_diag_stdout
# MUTANT    csplit.repeat_removes_some
# MUTANT    csplit.repeat_removes_preexisting
# MUTANT    csplit.missing_status0
# MUTANT    csplit.missing_no_diag
# MUTANT    csplit.missing_diag_stdout
# MUTANT    csplit.ln_end_plus1_ok
# MUTANT    csplit.accept_backward
# MUTANT    csplit.rep_nomatch_ok
# VARIANT   csplit.err_status2
# VARIANT   csplit.err_no_sizes
# VARIANT   csplit.err_extra_diag
. "$PC_CORE/pc.sh"
pc_begin XCU.csplit.007
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '%s\n' 1 2 3 4 5 6 7 8 9 10 >in10 || pc_fail "cannot create input"
printf 'h\ns1\na\ns2\nb\ns3\nc\n' >f3 || pc_fail "cannot create input"
pc_test

# expect_error LABEL ARG...: csplit with the arguments must fail, say why on
# standard error, and leave no xx00 to xx03; a file that was there before
# (xx09) and the input stay.
expect_error() {
	label=$1
	shift
	rm -f xx00 xx01 xx02 xx03 err
	printf 'kept\n' >xx09
	csplit "$@" >/dev/null 2>err </dev/null
	rc=$?
	[ "$rc" -ne 0 ] || pc_fail "$label: status 0"
	[ -s err ] || pc_fail "$label: no diagnostic on standard error"
	for f in xx00 xx01 xx02 xx03; do
		[ -e "$f" ] && pc_fail "$label: $f was left after the error"
	done
	[ "$(cat xx09)" = kept ] || pc_fail "$label: a file that was there before was removed or changed"
	[ -s in10 ] || pc_fail "$label: the input was removed"
	rm -f xx09
}

expect_error "no match after two pieces" -s in10 3 6 '/nomatchatall/'
expect_error "no match for a skip" in10 '%nomatchatall%'
expect_error "line number past the end" -s in10 3 6 20
expect_error "the line after the last" -s in10 3 6 11
expect_error "line number behind the current line" -s in10 3 6 5
expect_error "repeat of a regular expression that runs out" -s f3 '/^s/' '{5}'
expect_error "repeat of a line number that runs out" -s in10 3 '{9}'
expect_error "input file that does not exist" -s nosuchfile 3
pc_pass
