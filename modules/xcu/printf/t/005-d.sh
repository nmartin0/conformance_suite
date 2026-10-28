# %d and %i.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.printf.005
. "$PC_CORE/pc.sh"
pc_begin XCU.printf.005
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
pf 42 '%d' 42
pf -5 '%d' -5
pf 7 '%d' +7
pf 0 '%d' 0
pf 31 '%d' 0x1f
pf 31 '%d' 0X1F
pf 8 '%d' 010
pf 42 '%i' 42
pf -17 '%i' -17
pf 65 '%d' "'A"
pf 65 '%d' '"A'
pf 98 '%i' "'b"
pf "1 2 3" '%d %d %d' 1 2 3
pc_pass
