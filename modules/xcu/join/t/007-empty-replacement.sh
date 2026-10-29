# join -e gives the string to use for -o fields that are empty or missing.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.join.007
. "$PC_CORE/pc.sh"
pc_begin XCU.join.007
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a 1\nb 2\nc 3\n' >f1 && printf 'a x\nb y\nd z\n' >f2 || pc_fail "cannot create input"
pc_test
got=$(join -a 1 -e NONE -o 0,1.2,2.2 f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a 1 x${NL}b 2 y${NL}c 3 NONE${NL}rc=0" ] || pc_fail "-a 1 -e NONE -o 0,1.2,2.2 gave the wrong result (got: $(printf %s "$got" | tr '\n' ' '))"
printf 'a:1:\n' >g1 && printf 'a:x\n' >g2 || pc_fail "cannot create input"
got=$(join -t : -e EMPTY -o 0,1.3,2.2 g1 g2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a:EMPTY:x${NL}rc=0" ] || pc_fail "-e did not replace an empty field"
pc_pass
