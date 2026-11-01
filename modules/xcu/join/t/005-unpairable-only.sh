# join -v n writes only the lines of file n that have no partner.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.join.005
# MUTANT    join.v1_ignored
# MUTANT    join.v2_ignored
# MUTANT    join.v_swapped
# MUTANT    join.v_last_only
# MUTANT    join.v_with_pairs
# MUTANT    join.v_raw_line
# MUTANT    join.v_empty_status1
# MUTANT    join.adjacent_rejected
#
# Identical files leave nothing unpairable: the output is empty and the
# status is still zero. The adjacent form -v1 is the one the synopsis text
# itself uses for -a.
. "$PC_CORE/pc.sh"
pc_begin XCU.join.005
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a 1\nc 3\ne 5\ng 7\n' >f1 && printf 'b x\nc y\nf z\ng w\n' >f2 || pc_fail "cannot create input"
printf 'p1 a z1\np2 zz z2\n' >h1 && printf 'a 10\nb 20\n' >h2 || pc_fail "cannot create input"
pc_test
got=$(join -v 1 f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a 1${NL}e 5${NL}rc=0" ] || pc_fail "-v 1 gave the wrong result (got: $(printf %s "$got" | tr '\n' ' '))"
got=$(join -v 2 f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "b x${NL}f z${NL}rc=0" ] || pc_fail "-v 2 gave the wrong result (got: $(printf %s "$got" | tr '\n' ' '))"
got=$(join -v 1 -v 2 f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a 1${NL}b x${NL}e 5${NL}f z${NL}rc=0" ] || pc_fail "-v 1 -v 2 gave the wrong result (got: $(printf %s "$got" | tr '\n' ' '))"
got=$(join -v1 -v2 f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a 1${NL}b x${NL}e 5${NL}f z${NL}rc=0" ] || pc_fail "-v1 -v2 gave the wrong result (got: $(printf %s "$got" | tr '\n' ' '))"
got=$(join -1 2 -2 1 -v 1 h1 h2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "zz p2 z2${NL}rc=0" ] || pc_fail "-v 1 with -1 2 did not write the join field first (got: $(printf %s "$got" | tr '\n' ' '))"
got=$(join -v 1 f1 f1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "rc=0" ] || pc_fail "-v 1 on identical files gave output or a nonzero status"
pc_pass
