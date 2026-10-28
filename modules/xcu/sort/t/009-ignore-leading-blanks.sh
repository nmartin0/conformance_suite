# sort -b, and the b type on a key, ignore leading blanks when locating the start of a key.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sort.009
. "$PC_CORE/pc.sh"
pc_begin XCU.sort.009
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'p   zz\nq a\nr    mm\n' >in || pc_fail "cannot create input"
pc_test
got=$(sort -b -k 2,2 in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "q a${NL}r    mm${NL}p   zz${NL}rc=0" ] || pc_fail "-b -k 2,2 order is wrong"
got=$(sort -k 2b,2 in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "q a${NL}r    mm${NL}p   zz${NL}rc=0" ] || pc_fail "-k 2b,2 order is wrong"
pc_pass
