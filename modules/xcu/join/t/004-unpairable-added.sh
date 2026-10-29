# join -a n also writes the lines of file n that have no partner, in their place in the output.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.join.004
. "$PC_CORE/pc.sh"
pc_begin XCU.join.004
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a 1\nb 2\nc 3\n' >f1 && printf 'a x\nb y\nd z\n' >f2 || pc_fail "cannot create input"
pc_test
got=$(join -a 1 f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a 1 x${NL}b 2 y${NL}c 3${NL}rc=0" ] || pc_fail "-a 1 gave the wrong result (got: $(printf %s "$got" | tr '\n' ' '))"
got=$(join -a 2 f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a 1 x${NL}b 2 y${NL}d z${NL}rc=0" ] || pc_fail "-a 2 gave the wrong result (got: $(printf %s "$got" | tr '\n' ' '))"
got=$(join -a 1 -a 2 f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a 1 x${NL}b 2 y${NL}c 3${NL}d z${NL}rc=0" ] || pc_fail "-a 1 -a 2 gave the wrong result (got: $(printf %s "$got" | tr '\n' ' '))"
pc_pass
