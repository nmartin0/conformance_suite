# sort -b, and the b type on a key, ignore leading blanks when locating the start of a key.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sort.009
# MUTANT    sort.b_ignored
# MUTANT    sort.keyb_ignored
# MUTANT    sort.b_end_ignored
. "$PC_CORE/pc.sh"
pc_begin XCU.sort.009
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'p   zz\nq a\nr    mm\n' >in || pc_fail "cannot create input"
printf 'p   ac\nq ab\nr  ad\n' >in2 || pc_fail "cannot create input"
pc_test
got=$(sort -b -k 2,2 in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "q a${NL}r    mm${NL}p   zz${NL}rc=0" ] || pc_fail "-b -k 2,2 order is wrong"
got=$(sort -k 2b,2 in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "q a${NL}r    mm${NL}p   zz${NL}rc=0" ] || pc_fail "-k 2b,2 order is wrong"
got=$(sort -k 2.1b,2.2b in2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "q ab${NL}p   ac${NL}r  ad${NL}rc=0" ] || pc_fail "-k 2.1b,2.2b: b was not applied to both ends"
pc_pass
