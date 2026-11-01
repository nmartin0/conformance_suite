# The arithmetic operators.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.expr.001
# MUTANT    expr.add_sub
# MUTANT    expr.sub_add
# MUTANT    expr.mul_add
# MUTANT    expr.div_mod
# MUTANT    expr.mod_div
# MUTANT    expr.div_round
# MUTANT    expr.prec_flat
# MUTANT    expr.prec_inverted
# MUTANT    expr.assoc_right
# MUTANT    expr.octal_leading
# MUTANT    expr.result_abs
# MUTANT    expr.no_newline
# MUTANT    expr.double_newline
# MUTANT    expr.out_stderr
. "$PC_CORE/pc.sh"
pc_begin XCU.expr.001
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
ex 5 0 2 + 3
ex -1 0 2 - 3
ex 6 0 2 '*' 3
ex 3 0 7 / 2
ex 1 0 7 % 3
ex 14 0 2 + 3 '*' 4
ex 5 0 10 - 3 - 2
ex 2 0 100 / 10 / 5
ex 2 0 2 '*' 3 % 4
ex 18 0 20 / 3 '*' 3
ex 14 0 2 '*' 4 + 6
ex 100 0 99 + 1
# digits with a leading zero are still decimal
ex 11 0 010 + 1
ex 8 0 007 + 1
pc_pass
