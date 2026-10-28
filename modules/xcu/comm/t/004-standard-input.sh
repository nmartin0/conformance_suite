# A - operand makes comm read standard input in place of file1 or file2.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.comm.004
. "$PC_CORE/pc.sh"
pc_begin XCU.comm.004
LC_ALL=POSIX
export LC_ALL
NL='
'
T=$(printf '\t')
printf 'a\nb\n' >f1 && printf 'b\nc\n' >f2 || pc_fail "cannot create input"
pc_test
got=$(comm - f2 <f1 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}${T}${T}b${NL}${T}c${NL}rc=0" ] || pc_fail "- as file1 did not read standard input"
got=$(comm f1 - <f2 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}${T}${T}b${NL}${T}c${NL}rc=0" ] || pc_fail "- as file2 did not read standard input"
pc_pass
