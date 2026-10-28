# By default expand replaces each tab by spaces up to the next tab stop, tab stops being every eighth column.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.expand.001
. "$PC_CORE/pc.sh"
pc_begin XCU.expand.001
LC_ALL=POSIX
export LC_ALL
NL='
'
T=$(printf '\t')
s7='       '
s8='        '
printf 'a%sb\n' "$T" >in1 && printf '%sx\n' "$T" >in2 && printf 'abcdefgh%si\n' "$T" >in3 && printf 'no tabs here\n' >in4 || pc_fail "cannot create input"
pc_test
got=$(expand in1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${s7}b${NL}rc=0" ] || pc_fail "a tab after one character was not seven spaces"
got=$(expand in2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "${s8}x${NL}rc=0" ] || pc_fail "a leading tab was not eight spaces"
got=$(expand in3 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "abcdefgh${s8}i${NL}rc=0" ] || pc_fail "a tab at a tab stop did not go to the next one"
got=$(expand in4 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "no tabs here${NL}rc=0" ] || pc_fail "a line without tabs was changed"
pc_pass
