# The : matching operator.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.expr.006
# MUTANT    expr.match_unanchored
# MUTANT    expr.match_len_plus1
# MUTANT    expr.match_fail_null
# MUTANT    expr.match_fail_status0
# MUTANT    expr.match_group_ignored
# MUTANT    expr.match_group_last
# MUTANT    expr.match_group_whole
# MUTANT    expr.match_group_fail_zero
# MUTANT    expr.match_ere
# MUTANT    expr.match_empty_error
# MUTANT    expr.colon_low_prec
. "$PC_CORE/pc.sh"
pc_begin XCU.expr.006
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
ex 1 0 abc : a
ex 3 0 abc : abc
ex 3 0 abc : '.*'
ex 2 0 abc : 'ab'
ex 0 1 abc : b
ex 0 1 abc : abcd
ex 0 1 abc : x
ex 3 0 abc123 : '[a-z]*'
ex 6 0 abc123 : '[a-z0-9]*'
ex 2 0 aaa : 'a\{2\}'
ex 0 1 '' : '.*'
ex b 0 abc : 'a\(b\)'
ex abc 0 abc : '\(.*\)'
ex ab 0 abc : '\(a.\)c'
ex a 0 abc : '\(a\)\(b\)'
ex '' 1 abc : '\(x\)'
ex 123 0 abc123 : '[a-z]*\([0-9]*\)'
# : binds tighter than the arithmetic operators
ex 6 0 3 + abc : abc
# the length of a variable, and the last segment of a path name, as the page shows them
ex 3 0 Xabc : '.*' - 1
ex file 0 /usr/abc/file : '.*/\(.*\)' '|' /usr/abc/file
ex file 0 file : '.*/\(.*\)' '|' file
# a group that matched the empty string gives the null string
ex '' 1 abc : 'a\(x*\)'
# the pattern is a basic regular expression: these characters are ordinary
ex 3 0 'a+b' : 'a+b'
ex 3 0 'a|b' : 'a|b'
# an operand that looks like an integer is still a string to match
ex 2 0 123 : 12
pc_pass
