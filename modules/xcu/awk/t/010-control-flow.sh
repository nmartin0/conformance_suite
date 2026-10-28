# if, while, do-while, for, break, continue, next and exit behave as specified, including exit running END and setting the status.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.010
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.010
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '1\n2\n3\n' >in || pc_fail "cannot create input"
pc_test
e() { awk "BEGIN { $1 }" 2>/dev/null </dev/null; }
[ "$(e 'if (1) print "y"; else print "n"; if (0) print "y"; else print "n"')" = "y${NL}n" ] || pc_fail "if/else wrong"
[ "$(e 'if ("0") print "T"; else print "F"; if ("") print "T"; else print "F"')" = "T${NL}F" ] || pc_fail "a string constant 0 is true and an empty string false"
[ "$(echo 0 | awk '{ if ($1) print "T"; else print "F" }' 2>/dev/null)" = "F" ] || pc_fail "a numeric-string field 0 was not false"
[ "$(echo 0.0 | awk '{ if ($1) print "T"; else print "F" }' 2>/dev/null)" = "F" ] || pc_fail "a numeric-string field 0.0 was not false"
[ "$(echo a | awk '{ if ($1) print "T"; else print "F" }' 2>/dev/null)" = "T" ] || pc_fail "a non-numeric field was not true"
[ "$(e 'i=0; while (i<3) { i++; s=s i }; print s')" = "123" ] || pc_fail "while wrong"
[ "$(e 'i=5; do { print i; i++ } while (i<3)')" = "5" ] || pc_fail "do-while did not run once"
[ "$(e 'for (i=0; i<3; i++) s=s i; print s')" = "012" ] || pc_fail "for wrong"
[ "$(e 'for (i=0; i<10; i++) { if (i==2) continue; if (i==5) break; s=s i }; print s')" = "0134" ] || pc_fail "break or continue wrong"
got=$(awk '$1 == 2 { next } { print }' in 2>/dev/null </dev/null)
[ "$got" = "1${NL}3" ] || pc_fail "next did not skip the rest of the program for the record"
got=$(awk 'BEGIN { print "a"; exit; print "b" } END { print "end" }' 2>/dev/null </dev/null)
[ "$got" = "a${NL}end" ] || pc_fail "exit in BEGIN did not skip the rest and run END"
awk 'BEGIN { exit 3 }' 2>/dev/null </dev/null
[ $? -eq 3 ] || pc_fail "exit 3 did not give status 3"
awk 'BEGIN { exit }' 2>/dev/null </dev/null
[ $? -eq 0 ] || pc_fail "exit with no expression did not give status 0"
got=$(awk '{ exit 4 } END { print "e" }' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "e${NL}rc=4" ] || pc_fail "exit in an action did not run END with status 4"
got=$(awk 'END { print "e1"; exit 2 } END { print "e2" }' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "e1${NL}rc=2" ] || pc_fail "exit in END did not stop further END actions"
awk '{ exit 6 } END { exit }' in >/dev/null 2>&1 </dev/null
[ $? -eq 6 ] || pc_fail "an exit with no expression in END changed the status"
got=$(awk '{ print; exit } END { print "done" }' in 2>/dev/null </dev/null)
[ "$got" = "1${NL}done" ] || pc_fail "exit did not stop reading input"
pc_pass
