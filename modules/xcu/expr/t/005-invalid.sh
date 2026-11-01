# Invalid expressions exit with status 2 or more.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.expr.005
# MUTANT    expr.invalid_exit1
# MUTANT    expr.invalid_exit0
# MUTANT    expr.invalid_exit3
# MUTANT    expr.invalid_no_diag
# MUTANT    expr.invalid_diag_stdout
# MUTANT    expr.div0_result0
# MUTANT    expr.div0_exit1
# MUTANT    expr.div0_no_diag
# MUTANT    expr.mod0_result0
# MUTANT    expr.mod0_exit1
# MUTANT    expr.mod0_no_diag
# MUTANT    expr.two_operands_first
# MUTANT    expr.open_paren_ok
# MUTANT    expr.close_paren_ok
# MUTANT    expr.empty_parens_ok
# MUTANT    expr.missing_operand_dropped
# VARIANT   expr.divzero_exit3
# VARIANT   expr.divzero_exit100
# VARIANT   expr.diag_multiline
. "$PC_CORE/pc.sh"
pc_begin XCU.expr.005
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
# exerr MIN MAX ARGS...: as exrc, and the error is reported on standard error
exerr() {
	lo=$1
	hi=$2
	shift 2
	for em in shell exec; do
		if [ "$em" = shell ]; then
			expr "$@" </dev/null >/dev/null 2>eerr
		else
			(exec expr "$@") </dev/null >/dev/null 2>eerr
		fi
		rc=$?
		pc_need_file eerr "the captured standard error"
		{ [ "$rc" -ge "$lo" ] && [ "$rc" -le "$hi" ]; } || pc_fail "expr $* ($em) exited $rc, wanted $lo to $hi"
		[ -s eerr ] || pc_fail "expr $* ($em) wrote no diagnostic to standard error"
	done
}
pc_test
exerr 2 2 1 +
exerr 2 2 1 '*'
exerr 2 2 1 2
# 1 + + 2 is not tried: the second + can be read as a string operand, and a string that is an operator symbol is not an argument the application can rely on
exerr 2 2 '(' 1
exerr 2 2 1 ')'
exerr 2 2 '(' ')'
exerr 2 125 1 / 0
exerr 2 125 1 % 0
pc_pass
