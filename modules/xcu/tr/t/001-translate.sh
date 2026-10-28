# tr string1 string2 replaces each character of string1 by the character in the same position of string2, ranges included.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tr.001
. "$PC_CORE/pc.sh"
pc_begin XCU.tr.001
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'aabbcc cab\n' >in || pc_fail "cannot create input"
pc_test
got=$(tr abc xyz <in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "xxyyzz zxy${NL}rc=0" ] || pc_fail "tr abc xyz gave the wrong result"
got=$(tr a-c A-C <in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "AABBCC CAB${NL}rc=0" ] || pc_fail "tr a-c A-C gave the wrong result"
got=$(tr a-b 'xy' <in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "xxyycc cxy${NL}rc=0" ] || pc_fail "tr a-b xy gave the wrong result"
pc_pass
