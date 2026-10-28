# Flags, width and precision for integers.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.printf.007
. "$PC_CORE/pc.sh"
pc_begin XCU.printf.007
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
pf "   42" '%5d' 42
pf "42   |" '%-5d|' 42
pf "00042" '%05d' 42
pf "+5" '%+d' 5
pf "-5" '%+d' -5
pf " 5" '% d' 5
pf "-5" '% d' -5
pf "007" '%.3d' 7
pf "  007" '%5.3d' 7
pf "010" '%#o' 8
pf "0xff" '%#x' 255
pf "0XFF" '%#X' 255
pf "-0042" '%05d' -42
pf "  +5" '%+4d' 5
pc_pass
