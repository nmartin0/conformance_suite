# A backspace is copied and lowers the column count used for the next tab stop.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.expand.004
. "$PC_CORE/pc.sh"
pc_begin XCU.expand.004
LC_ALL=POSIX
export LC_ALL
NL='
'
T=$(printf '\t')
BS=$(printf '\b')
printf 'ab%s%sc\n' "$BS" "$T" >in || pc_fail "cannot create input"
pc_test
got=$(expand in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "ab${BS}       c${NL}rc=0" ] || pc_fail "a tab after a backspace did not use the lowered column"
pc_pass
