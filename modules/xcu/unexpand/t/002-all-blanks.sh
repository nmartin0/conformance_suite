# unexpand -a also converts runs of two or more blanks that end at a tab stop, but not a single blank.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.unexpand.002
. "$PC_CORE/pc.sh"
pc_begin XCU.unexpand.002
LC_ALL=POSIX
export LC_ALL
NL='
'
T=$(printf '\t')
printf 'a       b\n' >in1 && printf 'abcdefg b\n' >in2 && printf 'ab  cd\n' >in3 && printf 'abcdef  gh\n' >in4 || pc_fail "cannot create input"
pc_test
got=$(unexpand -a in1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${T}b${NL}rc=0" ] || pc_fail "-a did not convert seven spaces ending at column 8"
got=$(unexpand -a in2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "abcdefg b${NL}rc=0" ] || pc_fail "-a converted a single blank before a tab stop"
got=$(unexpand -a in3 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "ab  cd${NL}rc=0" ] || pc_fail "-a converted blanks that do not reach a tab stop"
got=$(unexpand -a in4 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "abcdef${T}gh${NL}rc=0" ] || pc_fail "-a did not convert two spaces ending at column 8"
pc_pass
