# cut reads the files named as operands in order, and standard input for no operand or for -.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cut.011
. "$PC_CORE/pc.sh"
pc_begin XCU.cut.011
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a1\na2\n' >f1 || pc_fail "cannot create input"
printf 'b1\nb2\n' >f2 || pc_fail "cannot create input"
pc_test
got=$(cut -c 2 f1 f2 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "1${NL}2${NL}1${NL}2${NL}rc=0" ] || pc_fail "two file operands were not cut in order"
got=$(cut -c 1 <f2 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "b${NL}b${NL}rc=0" ] || pc_fail "standard input was not read without operands"
got=$(cut -c 1 f1 - f2 <f2 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}a${NL}b${NL}b${NL}b${NL}b${NL}rc=0" ] || pc_fail "- did not stand for standard input"
pc_pass
