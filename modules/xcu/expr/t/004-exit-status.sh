# The exit status follows the result.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.expr.004
# MUTANT    expr.status_always0
# MUTANT    expr.status_always1
# MUTANT    expr.status_inverted
# MUTANT    expr.zero_exit0
# MUTANT    expr.null_exit0
# MUTANT    expr.string_exit1
# MUTANT    expr.false_exit2
# MUTANT    expr.false_hidden
# MUTANT    expr.null_no_output
# MUTANT    expr.success_warning
. "$PC_CORE/pc.sh"
pc_begin XCU.expr.004
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
# quiet EXPR...: a successful evaluation (status 0 or 1) writes nothing to standard error
quiet() {
	for qm in shell exec; do
		if [ "$qm" = shell ]; then
			expr "$@" </dev/null >/dev/null 2>qerr
		else
			(exec expr "$@") </dev/null >/dev/null 2>qerr
		fi
		qrc=$?
		pc_need_file qerr "the captured standard error"
		{ [ "$qrc" -le 1 ] && [ ! -s qerr ]; } || pc_fail "expr $* ($qm) status $qrc, and it wrote to standard error"
	done
}
pc_test
ex 5 0 5
ex abc 0 abc
ex 0 1 0
ex '' 1 ''
ex 1 0 1
ex 0 1 3 - 3
ex 2 0 1 + 1
ex 0 1 1 = 2
quiet 5
quiet abc
quiet 0
quiet ''
quiet 3 - 3
quiet 1 = 2
pc_pass
