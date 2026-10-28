# uniq -f n ignores the first n fields (blanks plus non-blanks) in comparisons.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uniq.005
. "$PC_CORE/pc.sh"
pc_begin XCU.uniq.005
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'k1 same x\nk2 same x\nk3 other x\n' >in1 || pc_fail "cannot create input"
printf '  k1 same\n k2 same\nk3 same\n' >in2 || pc_fail "cannot create input"
pc_test
got=$(uniq -f 1 in1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "k1 same x${NL}k3 other x${NL}rc=0" ] || pc_fail "-f 1 did not skip the first field"
got=$(uniq -f 1 in2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "  k1 same${NL}rc=0" ] || pc_fail "-f 1 did not count leading blanks as part of a field"
got=$(uniq -f 2 in1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "k1 same x${NL}rc=0" ] || pc_fail "-f 2 did not skip two fields"
pc_pass
