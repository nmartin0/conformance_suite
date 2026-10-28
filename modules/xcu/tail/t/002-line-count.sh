# tail -n number starts relative to the end of the file, or from the numberth line when the number has a plus sign.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tail.002
. "$PC_CORE/pc.sh"
pc_begin XCU.tail.002
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '%s\n' 1 2 3 4 5 6 7 8 9 10 11 12 >f12 || pc_fail "cannot create input"
pc_test
got=$(tail -n 3 f12 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "10${NL}11${NL}12${NL}rc=0" ] || pc_fail "-n 3 did not write the last three lines"
got=$(tail -n -3 f12 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "10${NL}11${NL}12${NL}rc=0" ] || pc_fail "-n -3 did not write the last three lines"
got=$(tail -n +11 f12 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "11${NL}12${NL}rc=0" ] || pc_fail "-n +11 did not start at the eleventh line"
got=$(tail -n +1 f12 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1${NL}2${NL}3${NL}4${NL}5${NL}6${NL}7${NL}8${NL}9${NL}10${NL}11${NL}12${NL}rc=0" ] || pc_fail "-n +1 did not write the whole file"
got=$(tail -n 1 f12 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "12${NL}rc=0" ] || pc_fail "-n 1 did not write the last line"
pc_pass
