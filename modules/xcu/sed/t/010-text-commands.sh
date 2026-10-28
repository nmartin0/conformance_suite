# a, i and c write text, r copies a file (an unreadable one is empty), = writes the line number and w writes to a file.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sed.010
. "$PC_CORE/pc.sh"
pc_begin XCU.sed.010
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '1\n2\n3\n4\n' >in4 || pc_fail "cannot create input"
printf 'R\n' >rf || pc_fail "cannot create input"
[ ! -e nosuchfile ] || pc_unresolved "nosuchfile exists"
pc_test
got=$(sed '2a\
AFTER' in4 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1${NL}2${NL}AFTER${NL}3${NL}4${NL}rc=0" ] || pc_fail "a gave the wrong result"
got=$(sed '2i\
BEFORE' in4 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1${NL}BEFORE${NL}2${NL}3${NL}4${NL}rc=0" ] || pc_fail "i gave the wrong result"
got=$(sed '2c\
CHANGED' in4 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1${NL}CHANGED${NL}3${NL}4${NL}rc=0" ] || pc_fail "c on one line gave the wrong result"
got=$(sed '2,3c\
CHANGED' in4 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1${NL}CHANGED${NL}4${NL}rc=0" ] || pc_fail "c on a range did not write the text once at its end"
got=$(sed '1a\
one\
two' in4 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1${NL}one${NL}two${NL}2${NL}3${NL}4${NL}rc=0" ] || pc_fail "multi-line a text gave the wrong result"
got=$(sed '2r rf' in4 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1${NL}2${NL}R${NL}3${NL}4${NL}rc=0" ] || pc_fail "r gave the wrong result"
got=$(sed '2r nosuchfile' in4 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1${NL}2${NL}3${NL}4${NL}rc=0" ] || pc_fail "r of a missing file was not treated as empty"
got=$(sed -n '2{a\
A
r rf
p
}' in4 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "2${NL}A${NL}R${NL}rc=0" ] || pc_fail "queued a and r text was not written in order after the pattern space"
got=$(sed -n '$=' in4 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "4${NL}rc=0" ] || pc_fail "= did not write the line count"
got=$(sed '3q;=' in4 2>/dev/null </dev/null | while IFS= read -r l; do printf '%s,' "$l"; done)
[ "$got" = "1,1,2,2,3," ] || pc_fail "= or q gave the wrong result"
rm -f out
sed -n '2,3w out' in4 >/dev/null 2>&1 </dev/null
got=$(cat out 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "2${NL}3${NL}rc=0" ] || pc_fail "w did not write the selected lines to the file"
pc_pass
