# Without -b a tab advances to the next tab stop (columns 1, 9, 17, ...); with -b every byte counts as one.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.fold.003
. "$PC_CORE/pc.sh"
pc_begin XCU.fold.003
LC_ALL=POSIX
export LC_ALL
NL='
'
T=$(printf '\t')
printf '%sabcdefgh\n' "$T" >in1 || pc_fail "cannot create input"
printf 'ab%scd\n' "$T" >in2 || pc_fail "cannot create input"
pc_test
got=$(fold -w 10 in1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "${T}ab${NL}cdefgh${NL}rc=0" ] || pc_fail "a leading tab did not count as eight columns"
got=$(fold -b -w 5 in1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "${T}abcd${NL}efgh${NL}rc=0" ] || pc_fail "-b did not count the tab as one byte"
got=$(fold -w 8 in2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "ab${T}${NL}cd${NL}rc=0" ] || pc_fail "a tab after two characters did not advance to column 8"
got=$(fold -b -w 8 in2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "ab${T}cd${NL}rc=0" ] || pc_fail "-b -w 8 broke a five-byte line"
pc_pass
