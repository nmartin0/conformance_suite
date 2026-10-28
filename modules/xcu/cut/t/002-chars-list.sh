# cut -c selects characters by single numbers and the three range forms.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cut.002
. "$PC_CORE/pc.sh"
pc_begin XCU.cut.002
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'abcdefgh\nijklmnop\n' >in || pc_fail "cannot create input"
pc_test
got=$(cut -c 2 in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "b${NL}j${NL}rc=0" ] || pc_fail "-c 2 gave the wrong result"
got=$(cut -c 3-5 in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "cde${NL}klm${NL}rc=0" ] || pc_fail "-c 3-5 gave the wrong result"
got=$(cut -c -2 in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "ab${NL}ij${NL}rc=0" ] || pc_fail "-c -2 gave the wrong result"
got=$(cut -c 7- in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "gh${NL}op${NL}rc=0" ] || pc_fail "-c 7- gave the wrong result"
got=$(cut -c 1,4-5,8 in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "adeh${NL}lmp${NL}rc=0" ] || pc_fail "-c 1,4-5,8 gave the wrong result"
pc_pass
