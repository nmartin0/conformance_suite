# diff -b ignores white space at the end of lines and treats other runs of white space as equal, but a blank is still different from none.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.diff.007
. "$PC_CORE/pc.sh"
pc_begin XCU.diff.007
LC_ALL=POSIX
export LC_ALL
NL='
'
mkf() {
	n=$1
	shift
	: >"$n" || pc_unresolved "cannot create input"
	for l in "$@"; do printf '%s\n' "$l" >>"$n"; done
}
printf 'a  b\n' >w1 && printf 'a b\n' >w2 && printf 'a b   \n' >w3 && printf 'a b\n' >w4 && printf 'ab\n' >w5 || pc_unresolved "cannot create input"
printf '  x\ny\n' >w6 && printf ' x\ny\n' >w7 && printf 'x\ny\n' >w8 || pc_unresolved "cannot create input"
pc_test
got=$(diff -b w1 w2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "rc=0" ] || pc_fail "-b: runs of blanks of different length were not equal"
diff w1 w2 >/dev/null 2>&1 </dev/null
[ $? -eq 1 ] || pc_fail "without -b, runs of blanks of different length were equal"
got=$(diff -b w3 w4 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "rc=0" ] || pc_fail "-b: trailing white space was not ignored"
diff w3 w4 >/dev/null 2>&1 </dev/null
[ $? -eq 1 ] || pc_fail "without -b, trailing white space was ignored"
diff -b w2 w5 >/dev/null 2>&1 </dev/null
[ $? -eq 1 ] || pc_fail "-b: a blank was equal to no blank"
got=$(diff -b w6 w7 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "rc=0" ] || pc_fail "-b: leading runs of blanks of different length were not equal"
diff -b w7 w8 >/dev/null 2>&1 </dev/null
[ $? -eq 1 ] || pc_fail "-b: a leading blank was equal to none"
printf 'a\tb\n' >w9 && printf 'a   b\n' >w10 || pc_unresolved "cannot create input"
got=$(diff -b w9 w10 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "rc=0" ] || pc_fail "-b: a tab and blanks were not equal"
printf 'a b\nc d\ne\n' >x1 && printf 'a   b\nc  X\ne   \n' >x2 || pc_unresolved "cannot create input"
out=$(diff -b x1 x2 2>/dev/null </dev/null)
[ "$out" = "2c2${NL}< c d${NL}---${NL}> c  X" ] || pc_fail "-b: only the real difference should be reported"
pc_pass
