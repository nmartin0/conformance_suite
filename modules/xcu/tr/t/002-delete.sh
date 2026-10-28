# tr -d deletes every input character that is in string1.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tr.002
. "$PC_CORE/pc.sh"
pc_begin XCU.tr.002
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'education\nabc\n' >in || pc_fail "cannot create input"
pc_test
got=$(tr -d aeiou <in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "dctn${NL}bc${NL}rc=0" ] || pc_fail "tr -d aeiou gave the wrong result"
got=$(tr -d a-c <in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "edution${NL}${NL}rc=0" ] || pc_fail "tr -d a-c gave the wrong result"
pc_pass
