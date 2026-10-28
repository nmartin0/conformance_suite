# In string2, [x*n] stands for n copies of x, and [x*] for as many as are needed; a leading zero makes n octal.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tr.008
. "$PC_CORE/pc.sh"
pc_begin XCU.tr.008
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'abcd\n' >in1 || pc_fail "cannot create input"
printf 'abcdefghi\n' >in2 || pc_fail "cannot create input"
pc_test
got=$(tr abcd '[x*2]yz' <in1 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "xxyz${NL}rc=0" ] || pc_fail "tr abcd '[x*2]yz' gave the wrong result"
got=$(tr abcd '[x*]' <in1 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "xxxx${NL}rc=0" ] || pc_fail "tr abcd '[x*]' gave the wrong result"
got=$(tr abcd 'w[x*]' <in1 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "wxxx${NL}rc=0" ] || pc_fail "tr abcd 'w[x*]' gave the wrong result"
got=$(tr abcdefghi '[x*010]y' <in2 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "xxxxxxxxy${NL}rc=0" ] || pc_fail "tr with [x*010] did not repeat 8 times"
pc_pass
