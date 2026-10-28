# A backspace lowers the column count and a carriage return resets it to zero; no newline is added next to either.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.fold.005
. "$PC_CORE/pc.sh"
pc_begin XCU.fold.005
LC_ALL=POSIX
export LC_ALL
NL='
'
BS=$(printf '\b')
CR=$(printf '\r')
printf 'ab%scde\n' "$BS" >in1 || pc_fail "cannot create input"
printf 'abcd%sefgh\n' "$CR" >in2 || pc_fail "cannot create input"
printf 'abcd%sefghij\n' "$CR" >in3 || pc_fail "cannot create input"
pc_test
got=$(fold -w 3 in1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "ab${BS}cd${NL}e${NL}rc=0" ] || pc_fail "-w 3 on a line with a backspace is wrong"
got=$(fold -w 4 in2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "abcd${CR}efgh${NL}rc=0" ] || pc_fail "a carriage return did not reset the count"
got=$(fold -w 4 in3 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "abcd${CR}efgh${NL}ij${NL}rc=0" ] || pc_fail "-w 4 after a carriage return is wrong"
pc_pass
