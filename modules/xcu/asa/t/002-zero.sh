# A line beginning with 0 is preceded by an extra newline, so a blank line comes before it.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.asa.002
. "$PC_CORE/pc.sh"
pc_begin XCU.asa.002
LC_ALL=POSIX
export LC_ALL
NL='
'
printf ' a\n0b\n 0c\n' >in || pc_fail "cannot create input"
pc_test
got=$(asa <in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}${NL}b${NL}0c${NL}rc=0" ] || pc_fail "0 did not give a blank line before b, or a later 0 was treated as a control character"
got=$(printf '0x\n' | asa 2>/dev/null)
[ "$got" = "${NL}x" ] || pc_fail "a first line beginning with 0 did not give a newline then the rest"
got=$(printf '00\n' | asa 2>/dev/null)
[ "$got" = "${NL}0" ] || pc_fail "only the first character of a line was taken as the control character"
pc_pass
