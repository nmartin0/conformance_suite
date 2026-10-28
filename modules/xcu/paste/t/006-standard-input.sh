# A - operand makes paste read standard input one line at a time, circularly for each - operand.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.paste.006
. "$PC_CORE/pc.sh"
pc_begin XCU.paste.006
LC_ALL=POSIX
export LC_ALL
NL='
'
T=$(printf '\t')
printf 'a1\na2\n' >f1 || pc_fail "cannot create input"
printf '1\n2\n3\n4\n' >in || pc_fail "cannot create input"
pc_test
got=$(paste - - <in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "1${T}2${NL}3${T}4${NL}rc=0" ] || pc_fail "paste - - did not pair successive standard input lines"
got=$(paste - f1 - <in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "1${T}a1${T}2${NL}3${T}a2${T}4${NL}rc=0" ] || pc_fail "paste - f1 - did not interleave standard input lines"
pc_pass
