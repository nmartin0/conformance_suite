# join -1 and -2 choose the join field; the output has the join field first, then the fields before and after it.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.join.002
. "$PC_CORE/pc.sh"
pc_begin XCU.join.002
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'p1 a z1\np2 b z2\np3 e z3\n' >f1 && printf 'a 10\nb 20\nc 30\n' >f2 || pc_fail "cannot create input"
printf 'k a\nl b\n' >h1 && printf 'x a\ny b\n' >h2 || pc_fail "cannot create input"
pc_test
got=$(join -1 2 -2 1 f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a p1 z1 10${NL}b p2 z2 20${NL}rc=0" ] || pc_fail "-1 2 -2 1 gave the wrong result"
got=$(join -1 2 -2 2 h1 h2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a k x${NL}b l y${NL}rc=0" ] || pc_fail "-1 2 -2 2 gave the wrong result"
pc_pass
