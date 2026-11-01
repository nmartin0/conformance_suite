# The comparison operators.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.expr.002
# MUTANT    expr.lt_as_le
# MUTANT    expr.le_as_lt
# MUTANT    expr.gt_as_ge
# MUTANT    expr.ge_as_gt
# MUTANT    expr.eq_as_ne
# MUTANT    expr.ne_as_eq
# MUTANT    expr.lt_as_gt
# MUTANT    expr.le_as_ge
# MUTANT    expr.cmp_string_only
# MUTANT    expr.cmp_fold_case
# MUTANT    expr.cmp_first_char
# MUTANT    expr.cmp_mixed_zero
# MUTANT    expr.cmp_strings_equal
# MUTANT    expr.cmp_str_reversed
# MUTANT    expr.cmp_true_left
# MUTANT    expr.cmp_false_empty
. "$PC_CORE/pc.sh"
pc_begin XCU.expr.002
LC_ALL=POSIX
export LC_ALL
nl='
'
# ex WANT STATUS ARGS...: expr ARGS must write WANT (without its newline)
# and exit with STATUS, called by the shell and by a direct exec.
ex() {
	want=$1
	wrc=$2
	shift 2
	got=$(expr "$@" </dev/null 2>/dev/null; echo "x$?")
	[ "$got" = "${want}${nl}x${wrc}" ] || pc_fail "expr $* did not write '$want' with status $wrc"
	got=$( (exec expr "$@") </dev/null 2>/dev/null; echo "x$?")
	[ "$got" = "${want}${nl}x${wrc}" ] || pc_fail "exec expr $* did not write '$want' with status $wrc"
}
# exrc MINSTATUS MAXSTATUS ARGS...: expr ARGS must exit with a status in the range.
exrc() {
	lo=$1
	hi=$2
	shift 2
	expr "$@" </dev/null >/dev/null 2>&1
	rc=$?
	{ [ "$rc" -ge "$lo" ] && [ "$rc" -le "$hi" ]; } || pc_fail "expr $* exited $rc, wanted $lo to $hi"
	(exec expr "$@") </dev/null >/dev/null 2>&1
	rc=$?
	{ [ "$rc" -ge "$lo" ] && [ "$rc" -le "$hi" ]; } || pc_fail "exec expr $* exited $rc, wanted $lo to $hi"
}
pc_test
ex 1 0 1 '<' 2
ex 0 1 2 '<' 1
ex 0 1 2 '<' 2
ex 1 0 2 '<=' 2
ex 1 0 1 '<=' 2
ex 0 1 3 '<=' 2
ex 1 0 3 '>' 2
ex 0 1 2 '>' 3
ex 1 0 3 '>=' 3
ex 0 1 3 '>' 3
ex 0 1 3 '>=' 4
ex 1 0 2 = 2
ex 0 1 2 = 3
ex 1 0 2 '!=' 3
ex 0 1 2 '!=' 2
ex 1 0 10 '>' 9
ex 0 1 10 '<' 9
ex 1 0 abc '<' abd
ex 0 1 abd '<' abc
ex 1 0 abc = abc
ex 0 1 abc = abd
ex 1 0 b '>' a
ex 1 0 A '<' a
ex 1 0 abc '!=' abd
ex 0 1 abc '!=' abc
ex 1 0 ab '<' abc
# a string and an integer are compared as strings
ex 1 0 10 '<' abc
ex 0 1 abc '<' 10
# an operand of only digits is an integer whatever its leading zeros
ex 1 0 007 = 7
ex 0 1 007 '!=' 7
pc_pass
