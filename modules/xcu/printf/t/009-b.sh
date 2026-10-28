# %b interprets backslash escapes in its argument.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.printf.009
. "$PC_CORE/pc.sh"
pc_begin XCU.printf.009
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
pf "a${tab}b" '%b' 'a\tb'
pf "a${nl}b" '%b' 'a\nb'
pf "$bsl" '%b' '\\'
pf "a${bel}${bsp}${ff}${cr}${vt}" '%b' 'a\a\b\f\r\v'
pf "A" '%b' '\0101'
pf "AB" '%b' '\0101\0102'
pf "xAy" '%b' 'x\0101y'
pf "A1" '%b' '\01011'
pf 'a\tb' '%s' 'a\tb'
pf "a${tab}b" 'a%sb' "${tab}"
pc_pass
