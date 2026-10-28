# A non-numeric argument is diagnosed and the rest is processed.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.printf.014
. "$PC_CORE/pc.sh"
pc_begin XCU.printf.014
nl='
'
bsl='\'
tab=$(echo a | tr a '\011')
bel=$(echo a | tr a '\007')
bsp=$(echo a | tr a '\010')
ff=$(echo a | tr a '\014')
cr=$(echo a | tr a '\015')
vt=$(echo a | tr a '\013')
# pf WANT FORMAT [ARG...]: printf must write exactly WANT, called by the
# shell and by a direct exec, and exit zero.
pf() {
	want=$1
	shift
	got=$(printf "$@" </dev/null 2>/dev/null; echo "x$?")
	[ "$got" = "${want}x0" ] || pc_fail "printf $* did not write what was wanted, or did not exit zero"
	got=$( (exec printf "$@") </dev/null 2>/dev/null; echo "x$?")
	[ "$got" = "${want}x0" ] || pc_fail "exec printf $* did not write what was wanted, or did not exit zero"
}
pc_test
printf '%d\n' abc 7 >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "printf exited zero for the non-numeric argument abc"
[ -s err ] || pc_fail "no diagnostic was written for the non-numeric argument abc"
last=
while IFS= read -r line; do last=$line; done <out
[ "$last" = 7 ] || pc_fail "the argument after the bad one was not processed; last line is '$last'"
(exec printf '%d\n' abc 7) >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "exec printf exited zero for the non-numeric argument abc"
[ -s err ] || pc_fail "exec printf wrote no diagnostic for abc"
last=
while IFS= read -r line; do last=$line; done <out
[ "$last" = 7 ] || pc_fail "exec printf did not process the argument after the bad one; last line is '$last'"
pc_pass
