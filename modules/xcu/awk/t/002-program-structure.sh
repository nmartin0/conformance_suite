# awk runs BEGIN actions, then each record against each pattern in order, then END actions; a missing pattern matches all and a missing action prints the record.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.002
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.002
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '1\n2\n3\n' >in || pc_fail "cannot create input"
pc_test
got=$(awk 'BEGIN { print "b1" }
BEGIN { print "b2" }
END { print "e1" }
END { print "e2" }' in 2>/dev/null </dev/null)
[ "$got" = "b1${NL}b2${NL}e1${NL}e2" ] || pc_fail "BEGIN and END actions were not run in program order around the input"
got=$(awk 'END { print "e" }
BEGIN { print "b" }' in 2>/dev/null </dev/null)
[ "$got" = "b${NL}e" ] || pc_fail "an END before a BEGIN changed the order"
got=$(awk '$1 > 1' in 2>/dev/null </dev/null)
[ "$got" = "2${NL}3" ] || pc_fail "a pattern with no action did not print matching records"
got=$(awk '{ print "x" $1 }' in 2>/dev/null </dev/null)
[ "$got" = "x1${NL}x2${NL}x3" ] || pc_fail "an action with no pattern did not run for every record"
got=$(awk '$1 == 2 { print "two" }
$1 > 1 { print "gt1" }' in 2>/dev/null </dev/null)
[ "$got" = "two${NL}gt1${NL}gt1" ] || pc_fail "patterns were not applied in program order for each record"
got=$(awk 'BEGIN { print "only", NR }' 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "only 0${NL}rc=0" ] || pc_fail "a BEGIN-only program gave the wrong result"
got=$(awk 'END { print NR }' in 2>/dev/null </dev/null)
[ "$got" = "3" ] || pc_fail "an END-only program did not read the input first"
got=$(awk 'BEGIN { x = 1 } END { print x, NR }' in 2>/dev/null </dev/null)
[ "$got" = "1 3" ] || pc_fail "BEGIN with END did not read the input"
got=$(awk '/1/ { print "one" }
/3/' in 2>/dev/null </dev/null)
[ "$got" = "one${NL}3" ] || pc_fail "newline-separated items gave the wrong result"
pc_pass
