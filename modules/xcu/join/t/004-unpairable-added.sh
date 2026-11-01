# join -a n also writes the lines of file n that have no partner, in their place in the output.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.join.004
# MUTANT    join.a1_ignored
# MUTANT    join.a2_ignored
# MUTANT    join.a_swapped
# MUTANT    join.a_last_only
# MUTANT    join.a_as_v
# MUTANT    join.a_at_end
# MUTANT    join.a_twice
# MUTANT    join.a_drop_key
# MUTANT    join.a_raw_line
# MUTANT    join.a_empty_nothing
# MUTANT    join.adjacent_rejected
# MUTANT    join.pair_unmatched
#
# The unpairable lines are interleaved with the pairs in key order. The
# adjacent form -a1 is the one the synopsis text itself uses.
. "$PC_CORE/pc.sh"
pc_begin XCU.join.004
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a 1\nc 3\ne 5\ng 7\n' >f1 && printf 'b x\nc y\nf z\ng w\n' >f2 || pc_fail "cannot create input"
printf 'p1 a z1\np2 zz z2\n' >h1 && printf 'a 10\nb 20\n' >h2 || pc_fail "cannot create input"
printf 'a 1\n' >k1 && printf 'x a\ny b\n' >k2 || pc_fail "cannot create input"
: >empty || pc_fail "cannot create input"
pc_test
got=$(join -a 1 f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a 1${NL}c 3 y${NL}e 5${NL}g 7 w${NL}rc=0" ] || pc_fail "-a 1 gave the wrong result (got: $(printf %s "$got" | tr '\n' ' '))"
got=$(join -a 2 f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "b x${NL}c 3 y${NL}f z${NL}g 7 w${NL}rc=0" ] || pc_fail "-a 2 gave the wrong result (got: $(printf %s "$got" | tr '\n' ' '))"
got=$(join -a 1 -a 2 f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a 1${NL}b x${NL}c 3 y${NL}e 5${NL}f z${NL}g 7 w${NL}rc=0" ] || pc_fail "-a 1 -a 2 gave the wrong result (got: $(printf %s "$got" | tr '\n' ' '))"
got=$(join -a1 -a2 f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a 1${NL}b x${NL}c 3 y${NL}e 5${NL}f z${NL}g 7 w${NL}rc=0" ] || pc_fail "-a1 -a2 gave the wrong result (got: $(printf %s "$got" | tr '\n' ' '))"
got=$(join -1 2 -2 1 -a 1 h1 h2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a p1 z1 10${NL}zz p2 z2${NL}rc=0" ] || pc_fail "-a 1 with -1 2 did not write the join field first (got: $(printf %s "$got" | tr '\n' ' '))"
got=$(join -2 2 -a 2 k1 k2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a 1 x${NL}b y${NL}rc=0" ] || pc_fail "-a 2 with -2 2 did not write the join field first (got: $(printf %s "$got" | tr '\n' ' '))"
got=$(join -a 1 f1 empty 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a 1${NL}c 3${NL}e 5${NL}g 7${NL}rc=0" ] || pc_fail "-a 1 with an empty file2 gave the wrong result"
got=$(join -a 2 empty f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "b x${NL}c y${NL}f z${NL}g w${NL}rc=0" ] || pc_fail "-a 2 with an empty file1 gave the wrong result"
pc_pass
