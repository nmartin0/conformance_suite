# comm -1, -2 and -3 each leave out one column and remove the tab that column's presence would add to the others.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.comm.002
. "$PC_CORE/pc.sh"
pc_begin XCU.comm.002
LC_ALL=POSIX
export LC_ALL
NL='
'
T=$(printf '\t')
printf 'a\nb\nc\nd\n' >f1 && printf 'b\nc\ne\n' >f2 || pc_fail "cannot create input"
pc_test
got=$(comm -1 f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "${T}b${NL}${T}c${NL}e${NL}rc=0" ] || pc_fail "-1 gave the wrong result"
got=$(comm -2 f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}${T}b${NL}${T}c${NL}d${NL}rc=0" ] || pc_fail "-2 gave the wrong result"
got=$(comm -3 f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}d${NL}${T}e${NL}rc=0" ] || pc_fail "-3 gave the wrong result"
pc_pass
