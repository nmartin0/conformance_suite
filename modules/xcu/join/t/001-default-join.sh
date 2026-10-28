# join writes the join field, then the other fields of both files, separated by single spaces; runs of blanks are one separator.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.join.001
. "$PC_CORE/pc.sh"
pc_begin XCU.join.001
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a 1\nb 2\nc 3\n' >f1 && printf 'a x\nb y\nd z\n' >f2 || pc_fail "cannot create input"
printf '  a    1\nb\t2\n' >g1 && printf 'a x\nb   y\n' >g2 || pc_fail "cannot create input"
pc_test
got=$(join f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a 1 x${NL}b 2 y${NL}rc=0" ] || pc_fail "paired lines were not joined on the first field"
got=$(join g1 g2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a 1 x${NL}b 2 y${NL}rc=0" ] || pc_fail "leading blanks or runs of blanks were not treated as single separators"
pc_pass
