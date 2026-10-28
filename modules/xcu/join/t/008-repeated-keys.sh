# When a key occurs several times, join writes every combination of the other fields, in the order the lines were met.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.join.008
. "$PC_CORE/pc.sh"
pc_begin XCU.join.008
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a 1\na 2\nb 3\n' >f1 && printf 'a x\na y\nb z\n' >f2 || pc_fail "cannot create input"
pc_test
got=$(join f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a 1 x${NL}a 1 y${NL}a 2 x${NL}a 2 y${NL}b 3 z${NL}rc=0" ] || pc_fail "repeated keys did not give all combinations in order"
pc_pass
