# Labels of b, t and : that differ within their first 8 bytes are different labels.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sed.025
# MUTANT    sed.label_truncated_4
. "$PC_CORE/pc.sh"
pc_begin XCU.sed.025
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'x\ny\nz\n' >in || pc_fail "cannot create input"
pc_test
# Two labels that differ only in their eighth byte.
got=$(sed -e '/x/b abcdefg1' -e 's/$/-A/' -e ':abcdefg1' -e '/y/b abcdefg2' -e 's/$/-B/' -e ':abcdefg2' -e 's/$/-C/' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "x-B-C${NL}y-A-C${NL}z-A-B-C${NL}rc=0" ] || pc_fail "labels that differ in their eighth byte were not told apart (got: $got)"
# The same with t. The marks are made with y, which does not count as a
# substitution for t.
printf 'x-pq\ny-pq\nz-pq\n' >in2 || pc_fail "cannot create input"
got=$(sed -e 's/x/X/' -e 't abcdefg1' -e 'y/p/P/' -e ':abcdefg1' -e 's/y/Y/' -e 't abcdefg2' -e 'y/q/Q/' -e ':abcdefg2' in2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "X-pQ${NL}Y-Pq${NL}z-PQ${NL}rc=0" ] || pc_fail "t labels that differ in their eighth byte were not told apart (got: $got)"
pc_pass
