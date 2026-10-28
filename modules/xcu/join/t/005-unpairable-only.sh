# join -v n writes only the lines of file n that have no partner.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.join.005
. "$PC_CORE/pc.sh"
pc_begin XCU.join.005
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a 1\nb 2\nc 3\n' >f1 && printf 'a x\nb y\nd z\n' >f2 || pc_fail "cannot create input"
pc_test
got=$(join -v 1 f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "c 3${NL}rc=0" ] || pc_fail "-v 1 gave the wrong result"
got=$(join -v 2 f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "d z${NL}rc=0" ] || pc_fail "-v 2 gave the wrong result"
got=$(join -v 1 -v 2 f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "c 3${NL}d z${NL}rc=0" ] || pc_fail "-v 1 -v 2 gave the wrong result"
pc_pass
