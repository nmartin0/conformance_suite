# cut -f uses tab as the default delimiter and separates selected fields with one tab.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cut.006
. "$PC_CORE/pc.sh"
pc_begin XCU.cut.006
LC_ALL=POSIX
export LC_ALL
NL='
'
T=$(printf '\t')
printf 'a\tb\tc\td\n' >in || pc_fail "cannot create input"
pc_test
got=$(cut -f 2 in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "b${NL}rc=0" ] || pc_fail "-f 2 gave the wrong result"
got=$(cut -f 1,3-4 in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${T}c${T}d${NL}rc=0" ] || pc_fail "-f 1,3-4 did not give tab-separated a c d"
got=$(cut -f 3- in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "c${T}d${NL}rc=0" ] || pc_fail "-f 3- gave the wrong result"
pc_pass
