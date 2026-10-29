# join -o writes exactly the listed fields; 0 is the join field; the list may be comma- or blank-separated.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.join.006
. "$PC_CORE/pc.sh"
pc_begin XCU.join.006
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a 1 2\nb 3 4\n' >f1 && printf 'a x y\nb u v\n' >f2 || pc_fail "cannot create input"
pc_test
got=$(join -o 1.2,2.3,0 f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1 y a${NL}3 v b${NL}rc=0" ] || pc_fail "-o 1.2,2.3,0 gave the wrong result (got: $(printf %s "$got" | tr '\n' ' '))"
got=$(join -o "2.2 1.3" f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "x 2${NL}u 4${NL}rc=0" ] || pc_fail "-o '2.2 1.3' gave the wrong result (got: $(printf %s "$got" | tr '\n' ' '))"
got=$(join -o 1.1 f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}b${NL}rc=0" ] || pc_fail "-o 1.1 did not write only that field"
pc_pass
