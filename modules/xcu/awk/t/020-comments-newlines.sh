# comments, backslash-newline, newlines after tokens
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.020
# A comment runs to the end of the line and has no effect, a backslash-newline
# has no effect, and a newline may follow a comma, an open brace, && and ||, do,
# else and the closing parenthesis of if, for and while.
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.020
LC_ALL=POSIX
export LC_ALL
NL='
'
pc_test
got=$(awk 'BEGIN { # comment with } and " and /
	x = 1 + \
	    2
	if (x == 3 &&
	    1 ||
	    0)
		print "ok1"
	print "a",
	      "b"
	do
		y++
	while (y < 3)
	if (y == 3)
		print "ok2"
	else
		print "bad"
	for (i = 0; i < 2; i++)
		z = z i
	print z
	while (w < 1)
		w++
	print w
}' 2>/dev/null </dev/null)
[ "$got" = "ok1${NL}a b${NL}ok2${NL}01${NL}1" ] || pc_fail "comments or newline placement: $got"
got=$(awk 'BEGIN { print "x" } # trailing comment' 2>/dev/null </dev/null)
[ "$got" = "x" ] || pc_fail "a comment after the last action was not ignored"
got=$(awk 'BEGIN { print 1 \
}' 2>/dev/null </dev/null)
[ "$got" = "1" ] || pc_fail "a backslash-newline before the closing brace had an effect"
pc_pass
