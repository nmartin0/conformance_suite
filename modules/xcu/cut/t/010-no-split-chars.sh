# In a locale where every character is one byte, -n with -b leaves the selected bytes unchanged.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cut.010
. "$PC_CORE/pc.sh"
pc_begin XCU.cut.010
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'abcdefgh\n' >in || pc_fail "cannot create input"
pc_test
got=$(cut -n -b 2-4 in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "bcd${NL}rc=0" ] || pc_fail "-n -b 2-4 did not give bcd"
got=$(cut -b 3-,1 -n in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "acdefgh${NL}rc=0" ] || pc_fail "-b 3-,1 -n did not give acdefgh"
pc_pass
