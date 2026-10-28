# head -n number writes that many lines; a number larger than the file copies it whole without error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.head.002
. "$PC_CORE/pc.sh"
pc_begin XCU.head.002
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '%s\n' 1 2 3 4 5 6 7 8 9 10 11 12 >f12 || pc_fail "cannot create input"
pc_test
got=$(head -n 3 f12 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1${NL}2${NL}3${NL}rc=0" ] || pc_fail "-n 3 did not write exactly three lines"
got=$(head -n 1 f12 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1${NL}rc=0" ] || pc_fail "-n 1 did not write exactly one line"
got=$(head -n 12 f12 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1${NL}2${NL}3${NL}4${NL}5${NL}6${NL}7${NL}8${NL}9${NL}10${NL}11${NL}12${NL}rc=0" ] || pc_fail "-n 12 did not write the whole file"
got=$(head -n 50 f12 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1${NL}2${NL}3${NL}4${NL}5${NL}6${NL}7${NL}8${NL}9${NL}10${NL}11${NL}12${NL}rc=0" ] || pc_fail "-n 50 on a 12-line file was not the whole file with status 0"
pc_pass
