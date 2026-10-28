# NR, FNR, FILENAME, NF, OFS, ORS, OFMT, CONVFMT, SUBSEP, ENVIRON, ARGC, ARGV, RSTART and RLENGTH have the specified values and effects.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.006
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.006
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '1\n2\n' >f1 && printf '3\n' >f2 || pc_fail "cannot create input"
pc_test
got=$(awk '{ print NR, FNR, FILENAME }' f1 f2 2>/dev/null </dev/null)
[ "$got" = "1 1 f1${NL}2 2 f1${NL}3 1 f2" ] || pc_fail "NR, FNR or FILENAME wrong"
got=$(awk 'BEGIN { print NR, FNR, "[" FILENAME "]" }' f1 2>/dev/null </dev/null)
[ "$got" = "0 0 []" ] || pc_fail "NR, FNR or FILENAME not zero or unset in BEGIN"
got=$(awk 'END { print NR, FNR, FILENAME }' f1 f2 2>/dev/null </dev/null)
[ "$got" = "3 1 f2" ] || pc_fail "NR, FNR or FILENAME wrong in END"
got=$(printf 'a b c\n' | awk 'END { print NF }' 2>/dev/null)
[ "$got" = "3" ] || pc_fail "NF did not keep its value in END"
got=$(printf 'a b\n' | awk 'BEGIN { OFS = "-" } { print $1, $2 }' 2>/dev/null)
[ "$got" = "a-b" ] || pc_fail "OFS not used by print"
got=$(awk 'BEGIN { ORS = "|" } { print }' f1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1|2|rc=0" ] || pc_fail "ORS not used by print"
got=$(awk 'BEGIN { OFMT = "%.2f"; print 3.14159, 3.14159 "" }' 2>/dev/null </dev/null)
[ "$got" = "3.14 3.14159" ] || pc_fail "OFMT was not used by print only"
got=$(awk 'BEGIN { CONVFMT = "%.2g"; x = 3.14159; y = x ""; print y }' 2>/dev/null </dev/null)
[ "$got" = "3.1" ] || pc_fail "CONVFMT was not used for number to string conversion"
got=$(awk 'BEGIN { CONVFMT = "%.2g"; x = 12345678; y = x ""; print y }' 2>/dev/null </dev/null)
[ "$got" = "12345678" ] || pc_fail "an integral value was converted with CONVFMT instead of as an integer"
got=$(awk 'BEGIN { SUBSEP = ":"; a[1,2] = 1; for (k in a) print k }' 2>/dev/null </dev/null)
[ "$got" = "1:2" ] || pc_fail "SUBSEP was not used to join subscripts"
got=$(PCX_ENVTEST=hello awk 'BEGIN { print ENVIRON["PCX_ENVTEST"] }' 2>/dev/null </dev/null)
[ "$got" = "hello" ] || pc_fail "ENVIRON did not show an environment variable"
got=$(awk 'BEGIN { print ARGC, ARGV[1], ARGV[2] }' p q 2>/dev/null </dev/null)
[ "$got" = "3 p q" ] || pc_fail "ARGC or ARGV wrong"
got=$(awk 'BEGIN { ARGV[1] = "" } { print }' nosuchfile f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "3${NL}rc=0" ] || pc_fail "an ARGV element set to null was not skipped"
got=$(awk 'BEGIN { ARGC = 2 } { print }' f1 f2 2>/dev/null </dev/null)
[ "$got" = "1${NL}2" ] || pc_fail "lowering ARGC did not drop the later operands"
got=$(awk 'BEGIN { ARGV[ARGC++] = "f2" } { print }' f1 2>/dev/null </dev/null)
[ "$got" = "1${NL}2${NL}3" ] || pc_fail "an operand added to ARGV was not read"
got=$(awk 'BEGIN { delete ARGV[1] } { print }' nosuchfile f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "3${NL}rc=0" ] || pc_fail "a deleted ARGV element was not skipped"
got=$(awk 'BEGIN { print match("foobar", "o+"), RSTART, RLENGTH }' 2>/dev/null </dev/null)
[ "$got" = "2 2 2" ] || pc_fail "match, RSTART or RLENGTH wrong for a match"
got=$(awk 'BEGIN { print match("abc", "z"), RSTART, RLENGTH }' 2>/dev/null </dev/null)
[ "$got" = "0 0 -1" ] || pc_fail "match, RSTART or RLENGTH wrong for no match"
pc_pass
